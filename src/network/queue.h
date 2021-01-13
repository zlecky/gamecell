#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <list>
#include <mutex>
#include <atomic>
#include <thread>
#include <vector>
#include <condition_variable>

#include "utils.h"
#include "types.h"

namespace TinyNet {
    /**
     * TaskQueue class.
     * @tparam T
     */
    template <typename T>
    class TaskQueue : private std::mutex, private Noncopyable {
    public:
        static const int MaxWaitMs = std::numeric_limits<int>::max();

    public:
        explicit TaskQueue(size_t task_cap) : capacity_(task_cap), exit_(false) {}

    public:
        bool push(T&& task) {
            std::lock_guard<std::mutex> lock(*this);

            if (exit_ || (capacity_ && items_.size() >= capacity_))
                return false;

            items_.push_back(std::move(task));
            ready_.notify_all();
            return true;
        }

        T pop_wait(int wait_ms = MaxWaitMs) {
            std::unique_lock<std::mutex()> lock(*this);

            this->wait_ready(lock, wait_ms);
            if (items_.empty())
                return T();

            T ret = std::move(items_.front());
            items_.pop_front();
            return ret;
        }

        bool pop_wait(T* task, int wait_ms = MaxWaitMs) {
            std::unique_lock<std::mutex> lock(*this);

            this->wait_ready(lock, wait_ms);
            if (items_.empty())
                return false;

            *task = std::move(items_.front());
            items_.pop_front();
            return true;
        }

    public:
        size_t size() {
            std::lock_guard<std::mutex> lock(*this);
            return items_.size();
        }

        void exit() {
            exit_ = true;

            std::lock_guard<std::mutex> lock(*this);
            ready_.notify_all();
        }

        bool exited() const { return exit_; }

    private:
        void wait_ready(std::unique_lock<std::mutex>& lock, int wait_ms) {
            if (exit_ || !items_.empty())
                return;

            if (wait_ms == MaxWaitMs) {
                ready_.wait(lock, [this]{ return exit_ || !items_.empty(); });
            } else if (wait_ms > 0) {
                auto timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(wait_ms);
                while (ready_.wait_until(lock, timeout) != std::cv_status::timeout && items_.empty() && !exit_) {

                }
            }
        }

    private:
        size_t capacity_;
        std::list<T> items_;
        std::atomic<bool> exit_;
        std::condition_variable ready_;
    };

    /**
     * Thread pool class.
     */
    class ThreadPool : private Noncopyable {
    public:
        explicit ThreadPool(size_t thread_num, size_t task_cap = 0) : threads_(thread_num), tasks_(task_cap) {}
        virtual ~ThreadPool() = default;

    public:
        void start();
        void exit();
        void join();

    public:
        bool add(Callback& cb);
        bool add(Callback&& cb);

        size_t size() { return tasks_.size(); }

    private:
        TaskQueue<Callback> tasks_;
        std::vector<std::thread> threads_;
    };
}

#endif//__QUEUE_H__
#include "queue.h"

namespace TinyNet {
    void ThreadPool::start() {
        for (auto& t : threads_) {
            std::thread th([this]() {
                while (!tasks_.exited()) {
                    Callback cb;
                    if (tasks_.pop_wait(&cb, 5000)) {
                        cb();
                    }
                }
            });

            t.swap(th);
        }
    }

    void ThreadPool::exit() {
        tasks_.exit();
    }

    void ThreadPool::join() {
        for (auto& t : threads_) {
            t.join();
        }
    }

    bool ThreadPool::add(Callback &cb) {
        return this->add(Callback(cb));
    }

    bool ThreadPool::add(Callback &&cb) {
        return tasks_.push(std::move(cb));
    }
}
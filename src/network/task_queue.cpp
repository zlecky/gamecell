#include "task_queue.h"

namespace TinyNet {
    void TaskPool::start() {
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

    void TaskPool::exit() {
        tasks_.exit();
    }

    void TaskPool::join() {
        for (auto& t : threads_) {
            t.join();
        }
    }

    bool TaskPool::add(Callback &cb) {
        return this->add(Callback(cb));
    }

    bool TaskPool::add(Callback &&cb) {
        return tasks_.push(std::move(cb));
    }
}
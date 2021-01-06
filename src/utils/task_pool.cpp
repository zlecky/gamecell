#include "task_pool.h"

namespace Util {
    void TaskPool::start() {
        for (auto& t : threads_) {
            std::thread th([this]() {
                while (!tasks_.exited()) {
                    Task task;
                    if (tasks_.pop(&task, 5000)) {
                        task();
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

    bool TaskPool::add(Task& task) {
        return this->add(Task(task));
    }

    bool TaskPool::add(Task&& task) {
        return tasks_.push(std::move(task));
    }
}
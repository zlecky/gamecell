#ifndef __EVENT_CONTEXT_H__
#define __EVENT_CONTEXT_H__

#include "types.h"
#include "utils.h"
#include "queue.h"

namespace TinyNet {
    class EventContextBase : private Noncopyable {
    public:
        virtual EventContextPtr context() = 0;
    };

    class EventContext : public EventContextBase {
    public:
        explicit EventContext(int task_cap = 0);
        virtual ~EventContext();

    public:
        EventContextPtr context() override { return this; }

    public:
        void loop();
        void loop_once(int wait_ms);

        void exit();
        bool exited() const { return exit_; }

        void wakeup();

    public:
        void trigger(Callback&& cb);
        void trigger(const Callback& cb);

    private:
        EventPollerPtr poller_;
        std::atomic<bool> exit_;

        int wakeup_fds_[2] = {0};
        TaskQueue<Callback> tasks_;
    };

    class EventContexts : public EventContextBase {
    public:
        EventContexts(size_t num) : id_(0), contexts_(num) {};
        virtual ~EventContexts() = default;

    public:
        EventContextPtr context() override {
            int id = id_++;
            return &contexts_[id % contexts_.size()];
        }

    public:
        void loop();

        void exit() {
            for (auto& ct : contexts_) {
                ct.exit();
            }
        }

    private:
        std::atomic<int> id_;
        std::vector<EventContext> contexts_;
    };
}

#endif//__EVENT_CONTEXT_H__
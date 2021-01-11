#ifndef __EVENT_DISPATCHER_H__
#define __EVENT_DISPATCHER_H__

#include "types.h"
#include "utils.h"
#include "queues.h"

namespace TinyNet {
    class BaseDispatcher : private Noncopyable {
    public:
        virtual BaseDispatcher* dispatcher() = 0;
    };

    class EventDispatcher : public BaseDispatcher {
    public:
        explicit EventDispatcher(int task_cap = 0);
        virtual ~EventDispatcher();

    public:
        BaseDispatcher* dispatcher() override { return this; }

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
}

#endif//__EVENT_DISPATCHER_H__
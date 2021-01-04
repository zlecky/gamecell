#ifndef __EVENT_DISPATCHER_H__
#define __EVENT_DISPATCHER_H__

#include "defs.h"
#include "util.h"

namespace TinyNet {
    class DispatcherImp;

    class EventDispatcherBase : private Noncopyable {
    public:
        virtual EventDispatcherBase* dispatcher() = 0;
    };

    /**
     * Single thread's event dispatcher.
     */
    class EventDispatcher : public EventDispatcherBase {
    public:
        explicit EventDispatcher(int task_capacity = 0);
        virtual ~EventDispatcher();

    public:
        EventDispatcherBase* dispatcher() override { return this; }

    public:
        void loop();
        void loop_once(int wait_ms);

        EventDispatcher& exit();
        bool exited() const;

        void wakeup();

    public:
        bool cancel(TimerId tid);

        TimerId trigger_at(int64_t milli, Task&& task, int64_t interval = 0);
        TimerId trigger_at(int64_t milli, const Task& task, int64_t interval = 0);

        TimerId trigger_after(int64_t milli, Task&& task, int64_t interval = 0);
        TimerId trigger_after(int64_t milli, const Task& task, int64_t interval = 0);

    public:
        void safe_call(Task&& task);
        void safe_call(const Task& task);

    public:
        std::unique_ptr<DispatcherImp> imp_;
    };

    /**
     * Multi threads' event dispatcher.
     */
    class MultiEventDispatcher : public EventDispatcherBase {

    };
}

#endif//__EVENT_DISPATCHER_H__
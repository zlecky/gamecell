#include "event_dispatcher.h"

namespace TinyNet {
    /**
     * The class implements the EventDispatcher.
     */
    class DispatcherImp {
    public:
        EventPollerPtr poller_;
        EventDispatcherPtr dispatcher_;

        std::atomic<bool> exit_;
    };

    EventDispatcher::EventDispatcher(int task_capacity/* = 0*/) {

    }

    EventDispatcher::~EventDispatcher() {

    }

    void EventDispatcher::loop() {

    }

    void EventDispatcher::loop_once(int wait_ms) {

    }
}
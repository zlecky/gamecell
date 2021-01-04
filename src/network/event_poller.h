/** @brief   A poller of network event lib.
 *  @file    poller_base.h
 *  @author  zlecky
 *  @version v0.0.1
 *  @date    2020/12/25
 */

#ifndef __EVENT_POLLER_H__
#define __EVENT_POLLER_H__

#include <poll.h>
#include <sys/types.h>

#include <atomic>
#include <memory>

#include "defs.h"
#include "util.h"

namespace TinyNet {
    /// Max event num
    const int kMaxEvents  = 2000;
    /// Read event
    const int kReadEvent  = POLLIN;
    /// Write event
    const int kWriteEvent = POLLOUT;

    /**
     * Event poller class.
     */
    class EventPoller : private Noncopyable {
    public:
        static EventPoller* create_poller();

    public:
        EventPoller() : last_active_(-1) {
            static std::atomic<int64_t> id(0);
            id_ = ++id;
        }

        virtual ~EventPoller() = default;

    public:
        virtual void add(ChannelPtr ch) = 0;
        virtual void remove(ChannelPtr ch) = 0;
        virtual void update(ChannelPtr ch) = 0;

        virtual void loop_once(int wait_ms) = 0;

    public:
        int64_t id_;
        int last_active_;
    };
}

#endif//__EVENT_POLLER_H__
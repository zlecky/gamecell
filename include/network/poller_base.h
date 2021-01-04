/** @brief   A poller of network event lib.
 *  @file    poller_base.h
 *  @author  zlecky
 *  @version v0.0.1
 *  @date    2020/12/25
 */

#ifndef __POLLER_BASE_H__
#define __POLLER_BASE_H__

#include <poll.h>
#include <sys/types.h>

#include <atomic>

#include "util.h"

namespace TinyNet {
    class Channel;

    /// Max event num
    const int kMaxEvents  = 2000;
    /// Read event
    const int kReadEvent  = POLLIN;
    /// Write event
    const int kWriteEvent = POLLOUT;

    /**
     * Base poller class.
     */
    class BasePoller : public Noncopyable {
    public:
        BasePoller() : last_active_(-1) {
            static std::atomic<int64_t> id(0);
            id_ = ++id;
        }

        virtual ~BasePoller() = default;

    public:
        virtual void loop_once(int wait_ms) = 0;
        virtual void add_channel(Channel* ch) = 0;
        virtual void remove_channel(Channel* ch) = 0;
        virtual void update_channel(Channel* ch) = 0;

    public:
        int64_t id_;
        int last_active_;
    };

}

#endif//__POLLER_BASE_H__
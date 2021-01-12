#include "channel.h"

#include <atomic>
#include <unistd.h>

#include "event_poller.h"
#include "event_context.h"

namespace TinyNet {

    Channel::Channel(EventContextPtr dispatcher, int fd, int events) :
            context_(dispatcher), fd_(fd), events_(events) {
        static std::atomic<int64_t> id(0);
        id_ = ++id;
        //TODO
        //poller_ = base_->imp_->poller_;
        poller_->add(this);
    }

    Channel::~Channel() {
        this->close();
    }

    void Channel::enable_r(bool enable) {
        if (enable) {
            events_ |= kReadEvent;
        } else {
            events_ &= ~kReadEvent;
        }

        poller_->update(this);
    }

    void Channel::enable_w(bool enable) {
        if (enable) {
            events_ |= kWriteEvent;
        } else {
            events_ &= ~kWriteEvent;
        }

        poller_->update(this);
    }

    void Channel::enable_rw(bool readable, bool writable) {
        if (readable) {
            events_ |= kReadEvent;
        } else {
            events_ &= ~kReadEvent;
        }

        if (writable) {
            events_ |= kWriteEvent;
        } else {
            events_ &= ~kWriteEvent;
        }

        poller_->update(this);
    }

    bool Channel::is_readable() const {
        return events_ & kReadEvent;
    }

    bool Channel::is_writable() const {
        return events_ & kWriteEvent;
    }

    void Channel::close() {
        if (fd_ >= 0) {
            poller_->remove(this);

            ::close(fd_);
            fd_ = -1;

            handle_read();
        }
    }
}
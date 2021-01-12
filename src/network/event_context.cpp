#include "event_context.h"

#include <fcntl.h>
#include <unistd.h>

#include "channel.h"
#include "event_poller.h"

namespace TinyNet {
    EventContext::EventContext(int task_cap/* = 0*/) :
        poller_(EventPoller::create_poller()),
        tasks_(task_cap),
        exit_(false) {
        ::pipe(wakeup_fds_);
        Util::add_fd_flag(wakeup_fds_[0], FD_CLOEXEC);
        Util::add_fd_flag(wakeup_fds_[1], FD_CLOEXEC);

        auto* ch = new Channel(this, wakeup_fds_[0], kReadEvent);
        ch->on_read([=](){
            char buf[1024] = {0};
            int ret = (ch->fd() >= 0) ? ::read(wakeup_fds_[0], buf, sizeof(buf)) : 0;
            if (ret > 0) {
                Callback cb;
                while (tasks_.pop_wait(&cb, 0)) {
                    cb();
                }
            } else if (0 == ret) {
                delete ch;
            } else if (EINTR == errno) {
                //TODO
            } else {
                //TODO
            }
        });
    }

    EventContext::~EventContext() {
        delete poller_;
        ::close(wakeup_fds_[1]);
    }

    void EventContext::loop() {
        while (!exit_) {
            this->loop_once(10000);
        }

        this->loop_once(0);
    }

    void EventContext::loop_once(int wait_ms) {
        poller_->loop_once(wait_ms);
    }

    void EventContext::exit() {
        exit_ = true;

        this->wakeup();
    }

    void EventContext::wakeup() {
        int ret = ::write(wakeup_fds_[1], "", 1);
    }

    void EventContext::trigger(Callback&& cb) {
        tasks_.push(std::move(cb));

        this->wakeup();
    }

    void EventContext::trigger(const Callback& cb) {
        this->trigger(Callback(cb));
    }
}
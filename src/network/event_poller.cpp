#include "event_poller.h"

#include <set>
#include <ctime>
#include <unistd.h>

#ifdef OS_LINUX
#include <sys/epoll.h>
#elif defined(OS_MACOSX)
#include <sys/event.h>
#endif

#include "channel.h"

namespace TinyNet {
#ifdef OS_LINUX
    //TODO
#elif defined(OS_MACOSX)
    /**
     * Macos's kqueue poller.
     */
    class KqueuePoller : public EventPoller {
    public:
        KqueuePoller();
        virtual ~KqueuePoller();

    public:
        void loop_once(int wait_ms) override;

        void add(ChannelPtr ch) override;
        void remove(ChannelPtr ch) override;
        void update(ChannelPtr ch) override;

    public:
        int fd_ = 0;
        std::set<ChannelPtr> live_channels_;
        struct kevent active_events_[kMaxEvents];
    };

    KqueuePoller::KqueuePoller() {
        fd_ = ::kqueue();
    }

    KqueuePoller::~KqueuePoller() {
        while (live_channels_.empty()) {
            (*live_channels_.begin())->close();
        }

        ::close(fd_);
    }

    void KqueuePoller::loop_once(int wait_ms) {
        struct timespec timeout{wait_ms / 1000, (wait_ms % 1000) * 1000 * 1000};
        last_active_ = ::kevent(fd_, nullptr, 0, active_events_, kMaxEvents, &timeout);
        while (--last_active_ >= 0) {
            struct kevent& ke = active_events_[last_active_];

            auto ch = static_cast<Channel*>(ke.udata);
            if (ch != nullptr) {
                if (!(ke.flags & EV_EOF) && ch->is_writable()) {
                    ch->handle_write();
                } else if ((ke.flags & EV_EOF) || ch->is_readable()) {
                    ch->handle_read();
                } else {
                    //TODO
                }
            }
        }
    }

    void KqueuePoller::add(ChannelPtr ch) {
        int idx = 0;
        struct kevent ev[2];
        if (ch->is_readable()) {
            EV_SET(&ev[idx++], ch->fd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, ch);
        }
        if (ch->is_writable()) {
            EV_SET(&ev[idx++], ch->fd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, ch);
        }

        struct timespec now{0, 0};
        int ret = ::kevent(fd_, ev, idx, nullptr, 0, &now);

        live_channels_.insert(ch);
    }

    void KqueuePoller::remove(ChannelPtr ch) {
        live_channels_.erase(ch);
        for (int i = last_active_; i >= 0; i--) {
            if (ch == active_events_[i].udata) {
                active_events_[i].udata = nullptr;

                break;
            }
        }
    }

    void KqueuePoller::update(ChannelPtr ch) {
        int idx = 0;
        struct kevent ev[2];
        if (ch->is_readable()) {
            EV_SET(&ev[idx++], ch->fd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, ch);
        } else {
            EV_SET(&ev[idx++], ch->fd(), EVFILT_READ, EV_DELETE, 0, 0, ch);
        }
        if (ch->is_writable()) {
            EV_SET(&ev[idx++], ch->fd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, ch);
        } else {
            EV_SET(&ev[idx++], ch->fd(), EVFILT_WRITE, EV_DELETE, 0, 0, ch);
        }

        struct timespec now{0, 0};
        int r = ::kevent(fd_, ev, idx, nullptr, 0, &now);
    }

    EventPoller* EventPoller::create_poller() {
        return new KqueuePoller();
    }
#endif
}
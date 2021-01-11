#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <memory>
#include <functional>

#include "utils.h"
#include "types.h"

namespace TinyNet {
    /**
     * Channel.
     */
    class Channel : private Noncopyable {
    public:
        Channel(EventDispatcherPtr dispatcher, int fd, int events);
        virtual ~Channel();

    public:
        void on_read(const Callback& cb) { read_cb_ = cb; }
        void on_write(const Callback& cb) { write_cb_ = cb; }
        void on_read(Callback&& cb) { read_cb_ = std::move(cb); }
        void on_write(Callback&& cb) { write_cb_ = std::move(cb); }

        bool is_readable() const;
        bool is_writable() const;

        void enable_r(bool enable);
        void enable_w(bool enable);
        void enable_rw(bool readable, bool writable);

        void handle_read() { read_cb_(); }
        void handle_write() { write_cb_(); }

        void close();

    public:
        int fd() { return fd_; }
        short events() { return events_; }
        int64_t id() { return id_; }
        EventPollerPtr poller() { return poller_; }
        EventDispatcherPtr dispatcher() { return dispatcher_; }

    protected:
        int fd_;
        int events_;

        uint64_t id_;

        EventPollerPtr poller_;
        EventDispatcherPtr dispatcher_;

        std::function<void()> read_cb_;
        std::function<void()> write_cb_;
        std::function<void()> error_cb_;
    };
}

#endif//__CHANNEL_H__
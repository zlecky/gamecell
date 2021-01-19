#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include "types.h"
#include "utils.h"
#include "buffer.h"
#include "endpoint.h"

#include <memory>

namespace TinyNet {
    class TcpConn : public std::enable_shared_from_this<TcpConn>, private TinyNet::Noncopyable {
    public:
        enum class State : uint16_t {
            invalid     = 1,
            handshaking = 2,
            connected   = 3,
            closed      = 4,
            failed      = 5,
        };

    public:
        TcpConn();
        virtual ~TcpConn();

    public:
        void attach(EventContextPtr ct, EndpointPtr ep);

        void on_read(const TcpCallback& cb);
        void on_state(const TcpCallback& cb);

    public:
        void handle_read(const TcpConnPtr conn);
        void handle_write(const TcpConnPtr conn);

    public:
        EventContextPtr context() const { return context_; };
        State state() const { return state_; }

    private:
        EventContextPtr context_;
        EndpointPtr endpoint_;
        ChannelPtr channel_;
        State state_;

        TcpCallback read_cb_;
        TcpCallback state_cb_;

        Buffer in_;
        Buffer out_;
    };
}

#endif//__TCP_CONNECTION_H__
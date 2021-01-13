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
        EventContextPtr context() const { return context_; };
        State state() const { return state_; }

    public:
        TcpConn();
        virtual ~TcpConn();

    private:
        EventContextPtr context_;
        State state_;
        Buffer in_;
        Buffer out_;

    };
}

#endif//__TCP_CONNECTION_H__
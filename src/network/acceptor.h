#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "types.h"
#include "utils.h"
#include "endpoint.h"

namespace TinyNet {
    class TcpAcceptor : private Noncopyable {
    public:
        explicit TcpAcceptor(EventContextPtr dispatcher, const std::string& ip, const std::string& port);
        virtual ~TcpAcceptor();

    public:
        bool start();

    public:
        void handle_accept();

    private:
        EventContextPtr context_;

        Address addr_;
        Endpoint endpoint_;
        ChannelPtr listener_;
    };
}

#endif//__ACCEPTOR_H__
#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "types.h"
#include "utils.h"
#include "codec.h"
#include "endpoint.h"

namespace TinyNet {
    class TcpAcceptor : private Noncopyable {
    public:
        static TcpAcceptorPtr create_tcp_acceptor(EventContextBasePtr base, const std::string& ip, const std::string& port);

    public:
        TcpAcceptor(EventContextBasePtr base, const std::string& ip, const std::string& port);
        virtual ~TcpAcceptor();

    public:
        bool run();

    public:
        void on_read_cb(const TcpCallback& cb) { read_cb_ = cb; }
        void on_state_cb(const TcpCallback& cb) { state_cb_ = cb; }
        void on_create_cb(const std::function<std::shared_ptr<TcpConn> ()>& cb) { create_cb_ = cb; }

    public:
        Address addr() { return addr_; }
        Endpoint endpoint() { return endpoint_; }
        EventContextPtr context() { return context_; }

    protected:
        void handle_accept();

    private:
        EventContextPtr context_      = nullptr;
        EventContextBasePtr contexts_ = nullptr;

        Address addr_;
        Endpoint endpoint_;
        std::shared_ptr<Channel> channel_;

        TcpCallback read_cb_;
        TcpCallback state_cb_;
        std::function<std::shared_ptr<TcpConn> ()> create_cb_;

        MsgCallback msg_cb_;
        std::unique_ptr<CodecBase> codec_;
    };
}

#endif//__ACCEPTOR_H__
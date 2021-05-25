#include "acceptor.h"

#include <fcntl.h>

#include "channel.h"
#include "event_poller.h"
#include "event_context.h"
#include "tcp_connection.h"

namespace TinyNet {
    TcpAcceptorPtr TcpAcceptor::create_tcp_acceptor(EventContextBasePtr base, const std::string& ip, const std::string& port) {
        auto acceptor = std::make_shared<TcpAcceptor>(base, ip, port);
        if (nullptr == acceptor)
            return nullptr;

        if (!acceptor->run())
            return nullptr;

        return acceptor;
    }

    TcpAcceptor::TcpAcceptor(EventContextBasePtr base, const std::string& ip, const std::string& port)
        : contexts_(base), context_(base->context()), addr_(ip, port), create_cb_([]() { return std::make_shared<TcpConn>(); }) {

    }

    TcpAcceptor::~TcpAcceptor() {

    }

    bool TcpAcceptor::run() {
        endpoint_.socket(SOCK_STREAM);
        endpoint_.set_reuse_addr(true);
        endpoint_.set_reuse_port(true);
        endpoint_.add_fd_flag(FD_CLOEXEC);

        int ret = endpoint_.bind(addr_.port(), addr_.ip());
        if (ret) {
            endpoint_.close();
            return false;
        }

        ret = endpoint_.listen(5);
        if (ret) {
            endpoint_.close();
            return false;
        }

        channel_ = std::make_shared<Channel>(context_, endpoint_.fd(), kReadEvent);
        if (nullptr == channel_) {
            endpoint_.close();
            return false;
        }

        channel_->on_read_cb([this]() {
            this->handle_accept();
        });

        return true;
    }

    void TcpAcceptor::handle_accept() {
        EndpointPtr ep = nullptr;
        while (channel_->fd() >= 0 && ((ep = endpoint_.accept()) != nullptr)) {
            ep->add_fd_flag(FD_CLOEXEC);

            auto* ct = contexts_->context();

            auto create_tcp_conn = [&]() {
                TcpConnPtr conn = this->create_cb_();
                if (nullptr == conn)
                    return;

                conn->attach(ct, ep);

                if (state_cb_ != nullptr) {
                    conn->on_state(state_cb_);
                }

                if (read_cb_ != nullptr) {
                    conn->on_read(read_cb_);
                }

                if (msg_cb_ != nullptr) {
                    conn->on_msg(codec_->clone(), msg_cb_);
                }
            };

            if (ct == context()) {
                create_tcp_conn();
            } else {
                ct->trigger(std::move(create_tcp_conn));
            }
        }
    }
}
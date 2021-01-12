#include "acceptor.h"

#include <fcntl.h>

#include "channel.h"
#include "event_poller.h"
#include "event_context.h"

namespace TinyNet {
    TcpAcceptor::TcpAcceptor(EventContextPtr dispatcher, const std::string& ip, const std::string& port)
        : context_(dispatcher), addr_(ip, port) {

    }

    TcpAcceptor::~TcpAcceptor() {

    }

    bool TcpAcceptor::start() {
        endpoint_.socket(SOCK_STREAM);
        endpoint_.set_reuse_addr(true);
        endpoint_.set_reuse_port(true);
        endpoint_.add_fd_flag(FD_CLOEXEC);

        int ret = endpoint_.bind(addr_.port(), addr_.ip());
        if (ret) {
            endpoint_.close();
            return false;
        }

        ret = endpoint_.listen(20);
        if (ret) {
            endpoint_.close();
            return false;
        }

        listener_ = new Channel(context_, endpoint_.fd(), kReadEvent);
        listener_->on_read([this]() {
            this->handle_accept();
        });

        return true;
    }

    void TcpAcceptor::handle_accept() {
        Endpoint* ep = nullptr;
        while (listener_->fd() >= 0 && ((ep = endpoint_.accept()) != nullptr)) {
            ep->add_fd_flag(FD_CLOEXEC);

        }
    }
}
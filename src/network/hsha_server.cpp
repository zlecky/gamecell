#include "hsha_server.h"

#include "acceptor.h"

namespace TinyNet {
    HsHaSrvPtr HsHaSrv::start_srv(EventContextBasePtr base, const std::string& ip, const std::string& port, size_t threads) {
        auto srv = std::make_shared<HsHaSrv>(threads);
        if (nullptr == srv)
            return nullptr;

        auto ap = TcpAcceptor::create_tcp_acceptor(base, ip, port);
        if (nullptr == ap)
            return nullptr;

        srv->acceptor(ap);

        return srv;
    }

    TcpAcceptorPtr HsHaSrv::acceptor() {
        return tcp_acceptor_;
    }

    void HsHaSrv::acceptor(TcpAcceptorPtr acceptor) {
        tcp_acceptor_ = acceptor;
    }

    void HsHaSrv::exit() {
        thread_pool_.exit();
        thread_pool_.join();
    }

    void HsHaSrv::handle_message(CodecBasePtr codec, const MsgRspCallback& cb) {
        //TODO
    }
}
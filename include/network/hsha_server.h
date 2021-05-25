#ifndef __HSHA_SERVER_H__
#define __HSHA_SERVER_H__

#include "utils.h"
#include "types.h"
#include "queue.h"
#include "codec.h"

namespace TinyNet {
    class HsHaSrv : private Noncopyable {
    public:
        static HsHaSrvPtr start_srv(EventContextBasePtr base, const std::string& ip, const std::string& port, size_t threads);

    public:
        explicit HsHaSrv(size_t threads) : thread_pool_(threads) {};

    public:
        TcpAcceptorPtr acceptor();
        void acceptor(TcpAcceptorPtr acceptor);

    public:
        void exit();

        void handle_message(CodecBasePtr codec, const MsgRspCallback& cb);

    private:
        ThreadPool thread_pool_;
        TcpAcceptorPtr tcp_acceptor_;
    };
}

#endif//__HSHA_SERVER_H__
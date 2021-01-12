#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include "types.h"
#include "utils.h"
#include "endpoint.h"

#include <memory>

namespace TinyNet {
    class TcpConn : public std::enable_shared_from_this<TcpConn>, private TinyNet::Noncopyable {
    public:
        TcpConn();
        virtual ~TcpConn();

    private:
        EventContextPtr context_;
        ChannelPtr channnel_;
    };

}

#endif//__TCP_CONNECTION_H__
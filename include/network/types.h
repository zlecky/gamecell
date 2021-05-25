#ifndef __TYPES_H__
#define __TYPES_H__

#include <map>

namespace TinyNet {
    class Endpoint;
    typedef std::shared_ptr<Endpoint> EndpointPtr;

    class Channel;
    typedef Channel* ChannelPtr;

    class EventPoller;
    typedef EventPoller* EventPollerPtr;

    class EventContext;
    typedef EventContext* EventContextPtr;
    class EventContextBase;
    typedef EventContextBase* EventContextBasePtr;

    class TcpConn;
    //typedef TcpConn* TcpConnPtr;
    typedef std::shared_ptr<TcpConn> TcpConnPtr;
    typedef std::function<void(const TcpConnPtr&)> TcpCallback;

    class Slice;
    typedef std::function<void(const TcpConnPtr&, Slice)> MsgCallback;

    class TcpAcceptor;
    typedef std::shared_ptr<TcpAcceptor> TcpAcceptorPtr;

    class HsHaSrv;
    typedef std::shared_ptr<HsHaSrv> HsHaSrvPtr;

    class CodecBase;
    typedef CodecBase* CodecBasePtr;

    typedef std::function<void()> Callback;
    typedef std::function<std::string(const TcpConnPtr&, const std::string&)> MsgRspCallback;
}


#endif//__TYPES_H__
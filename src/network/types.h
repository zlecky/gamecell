#ifndef __TYPES_H__
#define __TYPES_H__

#include <map>

namespace TinyNet {
    class Channel;
    typedef Channel* ChannelPtr;

    class EventPoller;
    typedef EventPoller* EventPollerPtr;

    class EventContext;
    typedef EventContext* EventContextPtr;

    class TcpConn;
    typedef TcpConn* TcpConnPtr;
    typedef std::function<void(const TcpConnPtr &)> TcpCallback;

    class TcpAcceptor;
    typedef TcpAcceptor* TcpAcceptorPtr;

    typedef std::function<void()> Callback;
}


#endif//__TYPES_H__
#ifndef __TYPES_H__
#define __TYPES_H__

#include <map>

namespace TinyNet {
    class Channel;
    class EventPoller;
    class EventDispatcher;

    typedef Channel* ChannelPtr;
    typedef EventPoller* EventPollerPtr;
    typedef EventDispatcher* EventDispatcherPtr;

    typedef std::function<void()> Callback;
    typedef std::pair<int64_t, int64_t> TimerId;
}


#endif//__TYPES_H__
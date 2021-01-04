#ifndef __DEFS_H__
#define __DEFS_H__

#include <map>

namespace TinyNet {
    class Channel;
    class EventPoller;
    class EventDispatcher;

    typedef Channel* ChannelPtr;
    typedef EventPoller* EventPollerPtr;
    typedef EventDispatcher* EventDispatcherPtr;

    typedef std::function<void()> Task;
    typedef std::pair<int64_t, int64_t> TimerId;
}


#endif//__DEFS_H__
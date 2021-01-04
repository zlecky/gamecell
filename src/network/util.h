#ifndef __UTIL_H__
#define __UTIL_H__

#include <sys/types.h>

namespace TinyNet {
    /**
     * Class forbid copy.
     */
    class Noncopyable {
    protected:
        Noncopyable() = default;
        virtual ~Noncopyable() = default;

        Noncopyable(const Noncopyable&) = delete;
        Noncopyable& operator=(const Noncopyable&) = delete;
    };


    /**
     * Time function helper.
     */
    struct TimeHelper {
        static int64_t timeMicro();
        static int64_t timeMilli() { return timeMicro() / 1000; }

        static int64_t steadyMicro();
        static int64_t steadyMilli() { return steadyMicro() / 1000; }
    };
}


#endif//__UTIL_H__
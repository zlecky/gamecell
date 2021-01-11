#ifndef __UTILS_H__
#define __UTILS_H__

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

    struct Timer {
        static int64_t timeMicro();
        static int64_t timeMilli() { return timeMicro() / 1000; }

        static int64_t steadyMicro();
        static int64_t steadyMilli() { return steadyMicro() / 1000; }
    };

    struct Util {
        static int add_fd_flag(int fd, int flag);
    };
}


#endif//__UTILS_H__
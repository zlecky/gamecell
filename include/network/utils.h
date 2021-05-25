#ifndef __UTILS_H__
#define __UTILS_H__

#include <sys/types.h>
#include <functional>

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

    struct Port {
        static const int kLittleEndian = LITTLE_ENDIAN;

        static uint16_t htobe(uint16_t v) {
            if (!kLittleEndian) {
                return v;
            }
            unsigned char *pv = (unsigned char *) &v;
            return uint16_t(pv[0]) << 8 | uint16_t(pv[1]);
        }
        static uint32_t htobe(uint32_t v) {
            if (!kLittleEndian) {
                return v;
            }
            unsigned char *pv = (unsigned char *) &v;
            return uint32_t(pv[0]) << 24 | uint32_t(pv[1]) << 16 | uint32_t(pv[2]) << 8 | uint32_t(pv[3]);
        }
        static uint64_t htobe(uint64_t v) {
            if (!kLittleEndian) {
                return v;
            }
            unsigned char *pv = (unsigned char *) &v;
            return uint64_t(pv[0]) << 56 | uint64_t(pv[1]) << 48 | uint64_t(pv[2]) << 40 | uint64_t(pv[3]) << 32 | uint64_t(pv[4]) << 24 | uint64_t(pv[5]) << 16 |
                   uint64_t(pv[6]) << 8 | uint64_t(pv[7]);
        }
        static int16_t htobe(int16_t v) {
            return (int16_t) htobe((uint16_t) v);
        }
        static int32_t htobe(int32_t v) {
            return (int32_t) htobe((uint32_t) v);
        }
        static int64_t htobe(int64_t v) {
            return (int64_t) htobe((uint64_t) v);
        }
    };

    struct Util {
        template <class T>
        static T hton(T v) {
            return Port::htobe(v);
        }
        template <class T>
        static T ntoh(T v) {
            return Port::htobe(v);
        }

        static int add_fd_flag(int fd, int flag);
    };

    struct Signal {
        static void signal(int sig, const std::function<void()>& handler);
    };
}


#endif//__UTILS_H__
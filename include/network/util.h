#ifndef __BASIC_TYPES_H__
#define __BASIC_TYPES_H__

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

}


#endif//__BASIC_TYPES_H__
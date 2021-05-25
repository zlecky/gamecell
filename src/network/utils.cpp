#include "utils.h"

#include <chrono>
#include <fcntl.h>
#include <map>

namespace TinyNet {
    int64_t Timer::timeMicro() {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    }
    int64_t Timer::steadyMicro() {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    }

    int Util::add_fd_flag(int fd, int flag) {
        int ret = ::fcntl(fd, F_GETFD);
        return ::fcntl(fd, ret | flag);
    }

    std::map<int, std::function<void()>> sig_handlers;

    void sig_handle(int sig) {
        sig_handlers[sig]();
    }

    void Signal::signal(int sig, const std::function<void()>& handler) {
        sig_handlers[sig] = handler;

        ::signal(sig, sig_handle);
    }
}
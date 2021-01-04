#include "util.h"

#include <chrono>

namespace TinyNet {
    int64_t TimeHelper::timeMicro() {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    }
    int64_t TimeHelper::steadyMicro() {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    }
}
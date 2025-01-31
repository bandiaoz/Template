#ifndef __POWER__
#define __POWER__

#include <cassert>

template <typename T>
constexpr static T power(T a, int64_t b) {
    assert(b >= 0);
    T res;
    if constexpr (std::is_arithmetic_v<T>) {
        res = 1;
    } else {
        res = a.identity();
    }
    while (b) {
        if (b & 1) {
            res = res * a;
        }
        b >>= 1;
        a = a * a;
    }
    return res;
}

#endif
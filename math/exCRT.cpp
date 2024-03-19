#include <bits/stdc++.h>
#include "exgcd.hpp"

/**
 * @brief Chinese Remainder Theorem
 * @brief x = reminder[i] (mod mod[i]), x is smallest positive integer solution
 * @param reminder: x = reminder[i] (mod mod[i])
 * @param mod: mod[i] don't need to be co-prime
 * @return pair(ok, x), x is smallest positive integer solution
 * @note lcm: lcm of mod
 * @if CRT: mod[i] is co-prime
 * @note m_i = \prod_{j \neq i} mod[j], c_i = m_i \cdot m_i^{-1}, m_i^{-1} is inverse of m_i modulo mod[i]
 * @note x = \sum reminder[i] \cdot c_i  \mod lcm
 * @else EXCRT: mod[i] don't need to be co-prime
 * @link CRT: https://www.luogu.com.cn/problem/P1495
 * @link EXCRT: https://www.luogu.com.cn/problem/P4777
 */
template<typename T>
static std::pair<bool, int64_t> chinese(const std::vector<T> &reminder, const std::vector<T> &mod) {
    int n = reminder.size();
    std::vector<T> coef(n, 1);
    int64_t x = 0, lcm = 1;
    for (int i = 0; i < n; i++) {
        int64_t a = coef[i] * lcm;
        int64_t b = reminder[i] - coef[i] * x;
        int64_t m = mod[i];
        auto [g, y, _] = exgcd(a, m);
        if (b % g) return {false, -1};
        b /= g; m /= g;
        x += lcm * (__int128_t(y) * b % m);
        lcm *= m;
    }
    x = (x + lcm) % lcm;
    return {true, x};
}

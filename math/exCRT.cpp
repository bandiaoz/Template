// CRT test problem: https://www.luogu.com.cn/problem/P1495
// EXCRT test problem: https://www.luogu.com.cn/problem/P4777

#include <bits/stdc++.h>

/**
 * @brief Extended Euclid Algorithm
 * @param a, b: input
 * @return tuple(g, x, y)
 * @note ax + by = gcd(a, b) = g
 * @note g = gcd(a, b) = gcd(b, a % b)
*/
std::tuple<int64_t, int64_t, int64_t> exgcd(int64_t a, int64_t b) {
    if (b == 0) {
        return std::tuple(a, 1, 0);
    }
    auto [g, x, y] = exgcd(b, a % b);
    return std::tuple(g, y, x - a / b * y);
}

/**
 * @brief Chinese Remainder Theorem
 * @param reminder: x = reminder[i] (mod mod[i])
 * @param mod: mod[i] don't need to be co-prime
 * @return pair(ok, x), x is smallest positive integer solution
 * @note lcm: lcm of mod
 * @if CRT: mod[i] is co-prime
 * @note m_i = \prod_{j \neq i} mod[j], c_i = m_i \cdot m_i^{-1}, m_i^{-1} is inverse of m_i modulo mod[i]
 * @note x = \sum reminder[i] \cdot c_i  \mod lcm
 * @else EXCRT: mod[i] don't need to be co-prime
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

#pragma once

#include <algorithm>
#include <cstdint>

namespace OY {
    struct ExtenedEuclideanTuple1 {
        int64_t m_gcd, m_coef1, m_coef2;
    };
    struct ExtenedEuclideanTuple2 {
        int64_t m_gcd, m_coef1;
        bool m_flag;
    };
    struct ExtenedEuclideanSolver {
        static ExtenedEuclideanTuple1 solve(uint64_t a, uint64_t b) {
            if (!a || !b) return {int64_t(a + b), 0, 0};
            int64_t s = b, t = a, m0 = 0, m1 = 1, m2 = 1, m3 = 0;
            while (t) {
                int64_t u = s / t;
                s -= t * u, m0 -= m1 * u, m2 -= m3 * u, std::swap(s, t), std::swap(m0, m1), std::swap(m2, m3);
            }
            return {s, m0, m2};
        }
        static ExtenedEuclideanTuple2 solve(int64_t a, int64_t b, int64_t target, int64_t coef1_thresh = 0) {
            if (!a && !b) return {0, 0, false};
            if (!b) return {a > 0 ? a : -a, target / a, target % a == 0};
            if (!a) return {b > 0 ? b : -b, 0, target % b == 0};
            int64_t s = b, t = a, m0 = 0, m1 = 1;
            while (t) {
                int64_t u = s / t;
                s -= t * u, m0 -= m1 * u, std::swap(s, t), std::swap(m0, m1);
            }
            if (target % s) return {s > 0 ? s : -s, 0, false};
            a /= s, b /= s;
            if (b < 0) a = -a, b = -b;
            m0 = m0 % b;
            if (m0 < 0) m0 += b;
            int64_t x = target / s % b;
            if (x < 0) x += b;
            m0 = __uint128_t(m0) * x % b;
            return {s > 0 ? s : -s, m0, true};
        }
    };
}
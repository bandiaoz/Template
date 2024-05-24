#include <bits/stdc++.h>
#include "../Others/Z.hpp"

using Z = ModInt<int, 998244353>;

/**
 * @brief Lagrange Interpolation
 * @param p: polynomial determined by n points, \forall p[i] = (x[i], y[i]), satisfy f(x[i]) = y[i]
 * @param k: the x value of the interpolation point
 * @return the value of poly in x = k, f(k)
 * @note f(k) = \sum_{i = 0}^{n} y_{i} \prod_{i \neq j} \frac{k - x[j]}{x[i] - x[j]}
 * @link https://www.luogu.com.cn/problem/P4781
*/
template<typename T>
Z lagrange(const std::vector<std::pair<T, T>> &p, Z k) {
    int n = p.size();
    Z ans = 0;
    for (int i = 0; i < n; i++) {
        Z s1 = p[i].second, s2 = 1;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                s1 *= k - p[j].first;
                s2 *= p[i].first - p[j].first;
            }
        }
        ans += s1 * s2.inv();
    }
    return ans;
}
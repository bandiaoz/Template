#include <bits/stdc++.h>
#include "../DataStruct/Fenwick.hpp"
#include "../Others/Z.hpp"

using Z = ModInt<int, 998244353>;

/**
 * @brief 康托展开 cantor
 * @param p is a permutation [0, n)
 * @note ord(p) = \sum A_i * (n - i - 1)!
 * @note A_i = \sum_{j > i} II(p[j] < p[i])
 * @return the order of p, ord in [0, n!)
 * @link https://www.luogu.com.cn/problem/P5367
 */
auto cantor(const std::vector<int> &p) -> Z {
    int n = p.size();
    std::vector<Z> fac{1};
    for (int i = 1; i < n; i++) {
        fac.push_back(fac.back() * i);
    }
    Z ans = 0;
    Fenwick<int> fen(n);
    for (int i = n - 1; i >= 0; i--) {
        ans += fen.sum(p[i]) * fac[n - i - 1];
        fen.add(p[i], 1);
    }
    return ans;
}

/**
 * @brief 逆康托展开 cantor_inv
 * @param A_i = \sum_{j > i} II(p[j] < p[i])
 * @note n \times n! > \sum_{i < n} i \times i!
 * @return p is a permutation [0, n)
 * @link https://www.luogu.com.cn/probslem/UVA11525
 * @link https://codeforces.com/contest/501/problem/D
 */
auto cantor_inv(std::vector<int> &A) -> std::vector<int> {
    int n = A.size();
    Fenwick<int> fen(n);
    for (int i = 0; i < n; i++) {
        fen.add(i, 1);
    }

    std::vector<int> p(n);
    for (int i = 0; i < n; i++) {
        p[i] = fen.kth(A[i]);
        fen.add(p[i], -1);
    }
    return p;
};

#include <bits/stdc++.h>

/**
 * @brief Sparse Table
 * @note O(nlogn) for build, O(1) for query
 * @note std::countl_zero(unsigned int) : Calculate the number of leading zeros
 * @note __builtin_clz(int) : Calculate the number of leading zeros
 * @link https://www.luogu.com.cn/problem/P3865
 * @example auto fun = [&](int i, int j) { return min(i, j); };
 *          SparseTable<int, decltype(fun)> st(a, fun);
 *      or:
 *          SparseTable<int> st(a, [&](int i, int j) { return min(i, j); });
*/
template <typename T, class F = std::function<T(const T&, const T&)>>
struct SparseTable {
    int n;
    std::vector<std::vector<T>> mat;
    F func;

    SparseTable(const std::vector<T>& a, const F& f) : func(f) {
        n = static_cast<int>(a.size());
        int max_log = std::__lg(n) + 1;
        mat.resize(max_log);
        mat[0] = a;
        for (int j = 1; j < max_log; j++) {
            mat[j].resize(n - (1 << j) + 1);
            for (int i = 0; i <= n - (1 << j); i++) {
                mat[j][i] = func(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    // return the answer [from, to)
    T get(int from, int to) const {
        assert(0 <= from && from <= to && to <= n);
        int lg = std::__lg(to - from);
        return func(mat[lg][from], mat[lg][to - (1 << lg)]);
    }
};
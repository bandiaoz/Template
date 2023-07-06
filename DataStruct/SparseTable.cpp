#include <bits/stdc++.h>

// usage:
//   auto fun = [&](int i, int j) { return min(i, j); };
//   SparseTable<int, decltype(fun)> st(a, fun);
// or:
//   SparseTable<int> st(a, [&](int i, int j) { return min(i, j); });
// __builtin_clz() : Calculate the number of leading zeros
// std::countl_zero() : Calculate the number of leading zeros

template <typename T, class F = std::function<T(const T&, const T&)>>
struct SparseTable {
    int n;
    std::vector<std::vector<T>> mat;
    F func;

    SparseTable(const std::vector<T>& a, const F& f) : func(f) {
        n = static_cast<int>(a.size());
        int max_log = 32 - std::countl_zero(unsigned(n));
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
        int lg = 32 - std::countl_zero(unsigned(to - from)) - 1;
        return func(mat[lg][from], mat[lg][to - (1 << lg)]);
    }
};
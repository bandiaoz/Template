#include <iostream>
#include <vector>
#include <cassert>

template <typename T>
struct pref_sum {
    int n;
    std::vector<T> sum;
    template <typename U>
    pref_sum(const std::vector<U>& a, T zero = T()) {
        init(a, [](T s, U x) { return s + x; }, zero);
    }
    template<typename U, typename F>
    pref_sum(const std::vector<U>& a, F func, T zero = T()) {
        init(a, func, zero);
    }
    template<typename U, typename F>
    void init(const std::vector<U>& a, F func, T zero = T()) {
        n = a.size();
        sum.assign(n + 1, zero);
        for (int i = 0; i < n; i++) {
            sum[i + 1] = func(sum[i], a[i]);
        }
    }
    // get sum of [from, to)
    T query(int from, int to) {
        assert(0 <= from && from <= to && to <= n);
        return sum[to] - sum[from];
    }
};
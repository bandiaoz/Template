#pragma once
#include <functional>
#include <limits>

/**
 * @brief 维护区间最大值
 */
template<class T>
struct InfoMax {
    static const T inf = std::numeric_limits<T>::max() / 2;
    T max;
    InfoMax(T max = -inf) : max(max) {}
    
    // merge(info, identity()) = info
    static InfoMax identity() { return InfoMax(-inf); }
    friend InfoMax operator+(const auto &lhs, const auto &rhs) {
        return InfoMax(std::max(lhs.max, rhs.max));
    }
};

/**
 * @brief 维护区间和
 * @link: https://www.luogu.com.cn/problem/P3374
 */
template<class T>
struct InfoSum {
    T sum;
    InfoSum(T sum = 0) : sum(sum) {}
    
    // merge(info, identity()) = info
    static InfoSum identity() { return InfoSum(0); }
    friend InfoSum operator+(const auto &lhs, const auto &rhs) {
        return InfoSum(lhs.sum + rhs.sum);
    }
};

/**
 * @brief 维护区间最大子段和(区间非空)
 * @link: https://www.luogu.com.cn/problem/P4513
 */
template<class T>
struct InfoMaxSubarray {
    static const T inf = std::numeric_limits<T>::max() / 2;
    T sum, left, right, ans;
    
    InfoMaxSubarray() {}
    InfoMaxSubarray(T x) : sum(x), left(x), right(x), ans(x) {}
    InfoMaxSubarray(T sum, T left, T right, T ans) : sum(sum), left(left), right(right), ans(ans) {}
    
    // merge(info, identity()) = info
    static InfoMaxSubarray identity() { return InfoMaxSubarray(0, -inf, -inf, -inf); }
    friend InfoMaxSubarray operator+(const auto &lhs, const auto &rhs) {
        return InfoMaxSubarray(
            lhs.sum + rhs.sum,
            std::max(lhs.left, lhs.sum + rhs.left),
            std::max(rhs.right, rhs.sum + lhs.right),
            std::max({lhs.ans, rhs.ans, lhs.right + rhs.left})
        );
    }
};
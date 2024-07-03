#pragma once
#include <bits/stdc++.h>

/**
 * @link https://www.luogu.com.cn/problem/P3373
 * @link https://judge.yosupo.jp/problem/range_affine_range_sum
 */
template <class T>
struct InfoSum {
    T val;
    size_t len;
    constexpr InfoSum(T val = 0, size_t len = 1) : val(val), len(len) {}

    // merge(info, identity()) = info
    static InfoSum identity() { return InfoSum(0, 0); }
    friend InfoSum operator+(const auto &A, const auto &B) {
        return InfoSum(A.val + B.val, A.len + B.len);
    }
};
template <class T>
struct TagMulAdd {
    T mul, add;
    constexpr TagMulAdd(T mul = 1, T add = 0) : mul(mul), add(add) {}

    // apply(info, identity()) = info
    static TagMulAdd identity() { return TagMulAdd(1, 0); }
    friend bool operator==(const auto &A, const auto &B) { return A.mul == B.mul && A.add == B.add; }
    friend bool operator!=(const auto &A, const auto &B) { return !(A == B); }
};
struct ApplyMulAdd {
    template <class Info, class Tag>
    static void applyInfo(Info &a, const Tag &b) {
        a.val = a.val * b.mul + b.add * a.len;
    }
    template <class Tag>
    static void applyTag(Tag &a, const Tag &b) {
        a.add = a.add * b.mul + b.add;
        a.mul *= b.mul;
    }
};

/**
 * @brief 维护区间最大子段和(区间非空)
 */
template <class T>
struct InfoMaxSubarray {
    static const T inf = std::numeric_limits<T>::max() / 2;
    T sum, left, right, ans;
    size_t len;

    InfoMaxSubarray() {}
    InfoMaxSubarray(T x) 
        : sum(x), left(x), right(x), ans(x), len(1) {}
    InfoMaxSubarray(T sum, T left, T right, T ans, size_t len) 
        : sum(sum), left(left), right(right), ans(ans), len(len) {}

    // merge(info, identity()) = info
    static InfoMaxSubarray identity() { return InfoMaxSubarray(0, -inf, -inf, -inf, 0); }
    friend InfoMaxSubarray operator+(const auto &lhs, const auto &rhs) {
        return InfoMaxSubarray(
            lhs.sum + rhs.sum, 
            std::max(lhs.left, lhs.sum + rhs.left),
            std::max(rhs.right, rhs.sum + lhs.right), 
            std::max({lhs.ans, rhs.ans, lhs.right + rhs.left})
        );
    }
};


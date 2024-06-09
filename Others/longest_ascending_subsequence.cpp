#include <bits/stdc++.h>

/**
 * @brief LIS 最长严格上升子序列
 * @link https://www.luogu.com.cn/problem/B3637
 */
int longest_ascending_subsequence(const std::vector<int> &a) {
    std::vector<int> f;
    for (auto x : a) {
        auto it = std::lower_bound(f.begin(), f.end(), x);
        if (it == f.end()) {
            f.push_back(x);
        } else {
            *it = x;
        }
    } 
    return f.size();
}
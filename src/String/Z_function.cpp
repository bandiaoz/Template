#include <bits/stdc++.h>

/**
 * @brief 定义函数 z[i] 表示 s 和 s[i:]（即以 s[i] 开头的后缀）的最长公共前缀（LCP）的长度。
 *        特别地，z[0] = 0。
 */
template<typename T>
std::vector<int> z_function(const T& s) {
    int n = (int)s.size();
    std::vector<int> z(n);
    for (int i = 1, j = 0; i < n; i++) {
        z[i] = std::max(0, std::min(j + z[j] - i, z[i - j]));
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > j + z[j]) j = i;
    }
    return z;
}
#include <bits/stdc++.h>

/**
 * @brief 最小表示法 minimal representation
 * @brief 找到所有循环同构的字符串中，最小的那一个
 * @tparam T std::vector<int> or std::string
 * @param s 
 * @return the minimal representation of s
 * @link https://www.luogu.com.cn/problem/P1368
 */
template <typename T>
T min_represent(const T &s) {
    int n = s.size();
    int i = 0, j = 1;
    for (int k = 0; k < n && i < n && j < n; ) {
        if (s[(i + k) % n] == s[(j + k) % n]) {
            k++;
        } else {
            s[(i + k) % n] > s[(j + k) % n] ? i = i + k + 1 : j = j + k + 1;
            if (i == j) i++;
            k = 0;
        }
    }
    i = std::min(i, j);

    T ans;
    for (int z = 0; z < n; z++) {
        ans.push_back(s[(i + z) % n]);
    }
    return ans;
}
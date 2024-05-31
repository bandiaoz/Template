#include <bits/stdc++.h>

/**
 * @brief 马拉车 manacher
 * @link https://www.luogu.com.cn/problem/P3805
 * @link https://judge.yosupo.jp/problem/enumerate_palindromes
 */
struct Manacher {
    int n;
    std::vector<int> res;
    Manacher() : n(0) {}
    Manacher(const auto &s) : n(s.size()) { init(s); }
    
    /**
     * @note res[2 * i] 以 i 为中心的回文串半径，回文串长度是奇数
     * @note res[2 * i + 1] 以 i 和 i + 1 为中心的回文串半径，回文串长度是偶数
     * @example s = "abaa" -> res = {0, 0, 1, 0, 0, 1, 0}
     * @example s = "aaa" -> res = {0, 1, 1, 1, 0}
     */
    void init(const auto &s) {
        if (n == 0) {
            res = std::vector<int>();
        }
        res.assign(2 * n - 1, 0);
        int l = -1, r = -1;
        for (int z = 0; z < 2 * n - 1; z++) {
            int i = (z + 1) >> 1;
            int j = z >> 1;
            int p = (i >= r ? 0 : std::min(r - i, res[2 * (l + r) - z]));
            while (j + p + 1 < n && i - p - 1 >= 0) {
                if (!(s[j + p + 1] == s[i - p - 1])) {
                    break;
                }
                p++;
            }
            if (j + p > r) {
                l = i - p;
                r = j + p;
            }
            res[z] = p;
        }
    }
    // 判断 s[l, r) 是否是回文串
    bool palindrome(int l, int r) {
        int len = r - l;
        int mid = (l + r - 1) / 2;
        if (len % 2 == 0) {
            return res[2 * mid + 1] * 2 >= len;
        } else {
            return res[2 * mid] * 2 + 1 >= len;
        }
    }
    std::pair<int, int> max_palindrome() {
        int len = 0, begin = -1;
        for (int z = 0; z < 2 * n - 1; z++) {
            if (z % 2 == 0 && 1 + 2 * res[z] > len) {
                len = 1 + 2 * res[z];
                begin = z / 2 - res[z];
            } else if (z % 2 == 1 && 2 * res[z] > len) {
                len = 2 * res[z];
                begin = z / 2 - res[z] + 1;
            }
        }
        return {begin, len};
    }
};
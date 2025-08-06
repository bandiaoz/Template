#include <bits/stdc++.h>
#include "src/Math/Poly/FFTPolynomial.h"
#ifdef LOCAL
#include "pprint.hpp"
#else
#define debug(...)
#endif

using ll = long long;
using poly = OY::FFTPolynomial<int>;

/*
[CF528D Fuzzy Search](https://codeforces.com/contest/528/problem/D)
[status](https://codeforces.com/contest/528/submission/307914387)
*/
/**
 * 给定两个仅包含 `A`, `C`, `G`, `T` 的字符串 s 和 t，问 t 在 s 中模糊匹配了多少次。
 * 如果 t 的第 i 个字符和 s 的第 [start + i - k, start + i + k] 个字符中，至少有一个字符相同，则认为 t 在 s 中出现了。
 * 
 * 对于每一个字符，计算 t 在 s.substr(start, m) 中的出现次数
 * 可以将 t 翻转，一共有 4 种字符，所以做 4 次 fft，将得到的多项式加起来，如果系数为 m，则认为 t 在 s 中出现了。
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, k;
    std::cin >> n >> m >> k;
    std::string s, t;
    std::cin >> s >> t;

    poly cnt;
    for (auto ch : std::string{"ACGT"}) {
        poly f(n), g(m);
        for (int i = 0, p = -1e9; i < n; i++) {
            if (s[i] == ch) p = i;
            if (i - p <= k) f[i] = 1;
        }
        for (int i = n - 1, p = 1e9; i >= 0; i--) {
            if (s[i] == ch) p = i;
            if (p - i <= k) f[i] = 1;
        }
        for (int i = 0; i < m; i++) {
            g[i] = t[m - 1 - i] == ch;
        }

        cnt += f * g;
    }

    std::cout << std::count(cnt.begin(), cnt.end(), m) << "\n";

    return 0;
}
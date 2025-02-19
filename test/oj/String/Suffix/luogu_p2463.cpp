#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"

using ll = long long;

/*
[P2463 [SDOI2008] Sandy 的卡片](https://www.luogu.com.cn/problem/P2463)
[status(SA)](https://www.luogu.com.cn/record/192916373)
*/
/**
 * 求 n 个串的最长公共子串，注意串由数字组成，并且可以偏移，也就是串 [1, 2, 3] 等价于串 [3, 4, 5]
 * 
 * SA：先差分，然后拼接起来，二分答案，判断是否存在长度为 mid 的公共子串
 * 也就是把 n 个串拼起来，中间插入不同的 inf 分隔符，查询 height >= mid 的区间，其中后缀的起点是否覆盖了所有的串
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<std::vector<int>> a(n);
    for (auto &v : a) {
        int m;
        std::cin >> m;
        std::vector<int> b(m);
        for (auto &x : b) {
            std::cin >> x;
        }
        v.resize(m - 1);
        for (int j = 0; j + 1 < m; j++) {
            v[j] = b[j + 1] - b[j];
        }
    }

    std::vector<int> s;
    constexpr int inf = 1e9;
    std::vector<int> begin;
    for (int i = 0; i < n; i++) {
        begin.push_back(s.size());
        s.insert(s.end(), a[i].begin(), a[i].end());
        if (i != n - 1) {
            s.push_back(inf + i);
        }
    }

    OY::SA::SuffixArray<true, true, 200'000> sa(s.begin(), s.end());
    auto group = [&](int rank) {
        auto it = std::upper_bound(begin.begin(), begin.end(), sa.query_sa(rank));
        return it - begin.begin() - 1;
    };
    auto judge = [&](int mid) -> bool {
        std::vector<int> exist(n, -1);
        for (int i = 0, j = 1; i < s.size(); i = j, j = i + 1) {
            int cnt = 0;
            auto add = [&](int rank) {
                if (exist[group(rank)] != i) {
                    cnt++;
                    exist[group(rank)] = i;
                }
            };
            while (j < s.size() && sa.query_height(j) >= mid) {
                add(j - 1), add(j);
                j++;
            }
            if (cnt == n) {
                return true;
            }
        }
        return false;
    };

    int l = 0, r = 1e9;
    while (l < r) {
        int mid = (l + r + 1) / 2;
        if (judge(mid)) {
            l = mid;
        } else {
            r = mid - 1;
        }
    }

    std::cout << l + 1 << '\n';

    return 0;
}
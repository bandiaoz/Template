#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"
#include "src/DataStruct/Segtree/CatTree.h"

/*
[P5341 [TJOI2019] 甲苯先生和大中锋的字符串](https://www.luogu.com.cn/problem/P5341)
[status(SA)](https://www.luogu.com.cn/record/192886462)
*/
/**
 * 找出恰好出现 k 次的子串集合，按照长度分类，求哪一个长度的子串最多，有相同输出最长的长度。
 * SA：对排名为 [i, i + k) 的后缀，这些后缀的 lcp > std::max(height[i], height[i + k])，也就是向前向后拓展都会使 lcp 变小
 *     那么长度为 [max + 1, lcp] 的子串都是满足条件的，差分维护即可。
 *     注意计算 lcp 时，特判 k = 1 的情况，直接用 n - sa[i] 即可。
 * SAM：
 */

void solve() {
    std::string s;
    int k;
    std::cin >> s >> k;
    int n = s.size();

    OY::SA::SuffixArray<true, true, 100000> sa(s.begin(), s.end());
    OY::CatMinTable<int> cat(n, [&](int i) { return sa.query_height(i); });
    std::vector<int> b(n + 2);
    for (int i = 0; i + k <= n; i++) {
        int lcp = (k == 1 ? n - sa.query_sa(i) : cat.query(i + 1, i + k));
        int max = sa.query_height(i);
        if (i + k < n) {
            max = std::max<int>(max, sa.query_height(i + k));
        }
        if (max + 1 <= lcp) {
            b[max + 1]++;
            b[lcp + 1]--;
        }
    }

    int len = 0;
    for (int l = 1; l <= n; l++) {
        b[l] += b[l - 1];
        if (b[l] >= b[len]) {
            len = l;
        }
    }

    std::cout << (b[len] == 0 ? -1 : len) << "\n";
}   

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
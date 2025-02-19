#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"
#include "src/DataStruct/RMQ/SparseTable.h"

/*
[P2852 [USACO06DEC] Milk Patterns G](https://www.luogu.com.cn/problem/P2852)
[status](https://www.luogu.com.cn/record/192167141)
*/
/**
 * 本题为经典子串问题，给定子串出现次数 $k(k >= 2)$，在至少出现 $k$ 次的子串里找最长，可以有多种做法
 * 
 * 可以使用后缀数组解决，相同子串对应的后缀排序后一定是连续的，所以可以维护长度为 $k$ 的区间，单调队列维护这个区间对应后缀的 lcp
 * 
 * 可以用字符串哈希解决
 * 可以使用后缀树解决
 * 可以使用后缀自动机解决
 */

void solve_sa() {
    int n, k;
    std::cin >> n >> k;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    OY::SA::SuffixArray<true, true, 20000> sa(a.begin(), a.end());
    int ans = 0;
    // 使用 RMQ 求 LCP
    // auto min = OY::STMinTable<int>(n, [&](size_t i) { return sa.query_height(i); });
    // 对于排名为 [l, r) 的后缀，求这些后缀的 lcp
    // for (int l = 0; l + k <= n; l++) {
    //     ans = std::max<int>(ans, min.query(l + 1, l + k));
    // }

    // 使用单调队列求 LCP
    std::deque<int> dq;
    for (int l = 0, r = 0; l + k <= n; l++) {
        while (r - l < k) {
            while (dq.size() && sa.query_height(dq.back()) >= sa.query_height(r)) {
                dq.pop_back();
            }
            dq.push_back(r++);
        }
        // 单调队列内的下标范围为 [l + 1, r)
        // 这样可以获得范围 [l, r) 的 lcp
        while (dq.size() && dq.front() < l + 1) {
            dq.pop_front();
        }
        ans = std::max<int>(ans, sa.query_height(dq.front()));
    }

    std::cout << ans << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_sa();

    return 0;
}
#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"
#include "src/DataStruct/DSU/LinearDSU.h"

using ll = long long;

/*
[P2178 [NOI2015] 品酒大会](https://www.luogu.com.cn/problem/P2178)
[status](https://www.luogu.com.cn/record/193084142)
*/
/**
 * 对于 r = 1...n，求有多少对后缀的 lcp 长度为 r，并求出这些后缀的最大权值乘积
 * 
 * 类似 P4248，用涨水法+并查集维护哪些后缀的 lcp 为 r，用并查集维护集合大小以及当前集合的 min/max
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::string s;
    std::cin >> s;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    OY::SA::SuffixArray<true, true, 300'000> sa(s.begin(), s.end());
    OY::LDSU::Table<true> dsu(n);
    const ll inf = 2e18;
    std::vector<int> min(n), max(n);
    for (int rank = 0; rank < n; rank++) {
        min[rank] = max[rank] = a[sa.query_sa(rank)];
    }

    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int i, int j) {
        return sa.query_height(i) > sa.query_height(j);
    });
    std::vector ans(n, std::pair{0LL, -inf});
    for (int i : order) {
        if (i - 1 >= 0) {
            int lcp = sa.query_height(i);
            ans[lcp].first += 1LL * dsu.size(i - 1) * dsu.size(i);

            int left_head = dsu.find_head(i - 1);
            int right_head = dsu.find_head(i);
            ans[lcp].second = std::max({
                ans[lcp].second,
                1LL * min[left_head] * min[right_head],
                1LL * max[left_head] * max[right_head]
            });
            min[left_head] = std::min(min[left_head], min[right_head]);
            max[left_head] = std::max(max[left_head], max[right_head]);
            dsu.unite_before(i);
        }
    }

    for (int i = n - 2; i >= 0; i--) {
        ans[i].first += ans[i + 1].first;
        ans[i].second = std::max(ans[i].second, ans[i + 1].second);
    }

    for (auto [cnt, max] : ans) {
        std::cout << cnt << ' ' << (cnt == 0 ? 0 : max) << '\n';
    }

    return 0;
}
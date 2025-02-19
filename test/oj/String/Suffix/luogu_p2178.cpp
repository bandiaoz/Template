#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"
#include "src/DataStruct/DSU/LinearDSU.h"
#include "src/DataStruct/RMQ/SparseTable.h"

using ll = long long;

/*
[P2178 [NOI2015] 品酒大会](https://www.luogu.com.cn/problem/P2178)
[status(dsu)](https://www.luogu.com.cn/record/193084142)
[status(stack)](https://www.luogu.com.cn/record/202510037)
*/
/**
 * 对于 r = 1...n，求有多少对后缀的 lcp 长度 >= r，并求出这些后缀的最大权值乘积
 * 
 * height 数组上，如果 min(height[i+1...j]) = r，则 lcp(substr(sa[i]), substr(sa[j])) = r
 * 
 * 类似 P4248，用涨水法+并查集维护哪些后缀的 lcp 为 r，用并查集维护集合大小以及当前集合的 min/max
 * 按照 height[rank] 从大到小的顺序遍历，此时如果选到的后缀分别来自 rank - 1 和 rank 所在的组，那么对应的 lcp = height[rank]
 * 也就是选到的两个后缀为 lcp = height[rank] 的方案数为 size(rank - 1) * size(rank)
 * 最大权值乘积为 max(min1 * min2, max1 * max2)，需要维护每个组的最小值和最大值
 * 
 * 也可以在 height 数组上使用单调栈，left[i] 表示 i 左边第一个「小于」 height[i] 的位置，right[i] 表示 i 右边第一个「小于等于」 height[i] 的位置
 * 注意 left[i] 初始化为 0，right[i] 初始化为 n
 * 如果选到的两个后缀分别来自 [left[rank], rank) 和 [rank, right[rank])，那么 lcp = height[rank]
 * 方案数为 (right[rank] - rank) * (rank - left[rank])
 * 最大权值乘积为 max(min1 * min2, max1 * max2)，用 st 表维护 min/max 即可
 */

void solve_dsu() {
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
}

void solve_stack() {
    int n;
    std::cin >> n;
    std::string s;
    std::cin >> s;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    OY::SA::SuffixArray<true, true, 300'000> sa(s.begin(), s.end());
    OY::STMaxTable<int> st_max(n, [&](size_t rank) { return a[sa.query_sa(rank)]; });
    OY::STMinTable<int> st_min(n, [&](size_t rank) { return a[sa.query_sa(rank)]; });
    
    const ll inf = 2e18;

    std::vector<int> stk;
    std::vector<int> left(n), right(n);
    for (int i = 0; i < n; i++) {
        while (stk.size() && sa.query_height(stk.back()) >= sa.query_height(i)) {
            stk.pop_back();
        }
        left[i] = stk.empty() ? 0 : stk.back();
        stk.push_back(i);
    }
    stk.clear();
    for (int i = n - 1; i >= 0; i--) {
        while (stk.size() && sa.query_height(stk.back()) > sa.query_height(i)) {
            stk.pop_back();
        }
        right[i] = stk.empty() ? n : stk.back();
        stk.push_back(i);
    }

    std::vector ans(n, std::pair{0LL, -inf});
    for (int rank = 1; rank < n; rank++) {
        int lcp = sa.query_height(rank);
        ans[lcp].first += 1LL * (right[rank] - rank) * (rank - left[rank]);

        ans[lcp].second = std::max(ans[lcp].second, 1LL * st_min.query(left[rank], rank) * st_min.query(rank, right[rank]));
        ans[lcp].second = std::max(ans[lcp].second, 1LL * st_max.query(left[rank], rank) * st_max.query(rank, right[rank]));
    }

    for (int i = n - 2; i >= 0; i--) {
        ans[i].first += ans[i + 1].first;
        ans[i].second = std::max(ans[i].second, ans[i + 1].second);
    }

    for (auto [cnt, max] : ans) {
        std::cout << cnt << ' ' << (cnt == 0 ? 0 : max) << '\n';
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // solve_dsu();
    solve_stack();

    return 0;
}
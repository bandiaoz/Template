#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"
#include "src/DataStruct/DSU/LinearDSU.h"

using ll = long long;

/*
[P4248 [AHOI2013] 差异] Milk Patterns G](https://www.luogu.com.cn/problem/P4248)
[status (两次单调栈)](https://www.luogu.com.cn/record/192426935)
[status (涨水法+并查集)](https://www.luogu.com.cn/record/193065738)
*/
/**
 * 令 diff(s, t) = |s| + |t| - 2 * lcp(s, t)
 * 求 \sum_{i < j} diff(s.substr(i), s.substr(j))，即求所有后缀两两之间的差异和
 * 
 * 结论：lcp(s.substr(sa[i]), s.substr(sa[j])) = min{height[i + 1], height[i + 2], ..., height[j]}
 * 感性理解：如果 height 一直大于某个数，前这么多位就一直没变过；反之，由于后缀已经排好序了，不可能变了之后变回来。
 * 
 * 故原题等价于求 lcp(s, t) = \sum_{i < j} min{height[i+1...j]}，由于 height[0] = 0, 所以等价于求 \sum_{i < j} min{height[i...j]}
 * 两次单调栈：维护 height 数组，left[i] 表示 i 左边第一个「小于」 height[i] 的位置，right[i] 表示 i 右边第一个「小于等于」 height[i] 的位置
 *        则 height[i] 对 lcp 的贡献为 (i - left[i]) * (right[i] - i) * height[i]
 * 单调栈：...
 * 涨水法+并查集：从大到小枚举 height，如果选到的后缀分别来自 rank - 1 和 rank 所在的组，那么对应的 lcp = height[rank]
 * 也就是选到的两个后缀为 lcp = height[rank] 的方案数为 size(rank - 1) * size(rank)，对 lcp 产生贡献为 size(rank - 1) * size(rank) * height[i]
 */

void solve_SA_stack2() {
    std::string s;
    std::cin >> s;
    int n = s.size();

    ll ans = 1LL * (n - 1) * n * (n + 1) / 2; // |s| + |t|

    OY::SA::SuffixArray<true, true, 500000> sa(s.begin(), s.end());
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

    for (int i = 1; i < n; i++) {
        // 2 * lcp(s, t)
        ans -= 2LL * (i - left[i]) * (right[i] - i) * sa.query_height(i);
    }

    std::cout << ans << '\n';
}

void solve_SA_dsu() {
    std::string s;
    std::cin >> s;
    int n = s.size();

    ll ans = 1LL * (n - 1) * n * (n + 1) / 2; // |s| + |t|
    
    OY::SA::SuffixArray<true, true, 500000> sa(s.begin(), s.end());
    OY::LDSU::Table<true> dsu(n);
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int i, int j) {
        return sa.query_height(i) > sa.query_height(j);
    });
    for (int i : order) {
        if (i - 1 >= 0) {
            int left = dsu.size(i - 1);
            int right = dsu.size(i);
            ans -= 2LL * left * right * sa.query_height(i);
            dsu.unite_before(i);
        }
    } 

    std::cout << ans << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_SA_stack2();
    // solve_SA_dsu();

    return 0;
}
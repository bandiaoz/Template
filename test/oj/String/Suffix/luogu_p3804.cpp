#include <bits/stdc++.h>
#include "src/String/Suffix/SAM.h"
#ifndef LOCAL
#define debug(...)
#endif

using ll = long long;

/*
[P3804 【模板】后缀自动机（SAM）](https://www.luogu.com.cn/problem/P3804)
[status(SAM)](https://www.luogu.com.cn/record/222161786)
*/
/**
 * 给定只包含小写字母的字符串 S，求 S 中所有出现次数不为 1 的子串中，出现次数乘以该子串长度的最大值
 * 可以使用后缀数组解决
 * 也可以在后缀自动机的 fail 树上解决
 * 也可以通过建立后缀树解决
 */

void solve_sam() {
    std::string s;
    std::cin >> s;

    OY::StaticSAM_string<26> sam(s.size(), [&](uint32_t i) { return s[i] - 'a'; });
    std::vector<int> cnt(s.size() * 2);
    for (int i = 0; i < s.size(); i++) {
        cnt[sam.query_node_index(i)] = 1;
    }

    sam.prepare();

    ll ans = 0;
    sam.do_for_failing_nodes([&](uint32_t u) {
        auto p = sam.get_node(u);
        if (cnt[u] > 1) {
            ans = std::max(ans, 1LL * cnt[u] * p->m_length);
        }
        if (~sam.query_fail(u)) {
            cnt[sam.query_fail(u)] += cnt[u];
        }
    });

    std::cout << ans << "\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_sam();

    return 0;
}
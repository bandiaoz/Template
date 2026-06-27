#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"
#include "src/String/Suffix/SAM.h"

using ll = long long;

/*
[Number of Substrings](https://judge.yosupo.jp/problem/number_of_substrings)(https://github.com/yosupo06/library-checker-problems/issues/123)
[status(SA)](https://judge.yosupo.jp/submission/248321)
[status(SAM)](https://judge.yosupo.jp/submission/297014)
[status(SAM_link)](https://judge.yosupo.jp/submission/297041)
*/
/**
 * 本题为后缀数组算法模板，计算字符串本质不同的子串数量
 * 
 * SA：枚举 rank，rank = i 的后缀贡献 n - sa[i] 个前缀，减去 height[i] 个和 rank = i - 1 的后缀的重复前缀
 * SAM：不同子串的数目等于自动机中以 $$t_0$$ 为起点的路径数量，SAM 是一个 DAG
 */

void solve_sa() {
    std::string s;
    std::cin >> s;
    int n = s.size();
    
    OY::SA::SuffixArray<true, true, 500000> SA(s.begin(), s.end(), 128);
    ll ans = 0;
    for (int i = 0; i < n; i++) {
        int can = n - SA.query_sa(i);
        int h = SA.query_height(i);
        ans += can - h;
    }
    
    std::cout << ans << "\n";
}

void solve_sam() {
    std::string s;
    std::cin >> s;
    
    OY::StaticSAM_string<26> sam(s.size(), [&](uint32_t i) { return s[i] - 'a'; });
    sam.prepare();

    std::vector<ll> cnt(sam.m_data.size()); // 以 u 为起点，长度至少为 1 的路径数量
    sam.do_for_failing_nodes([&](uint32_t u) {
        auto node = sam.get_node(u);
        for (int i = 0; i < 26; i++) {
            if (node->get_child(i)) {
                cnt[u] += cnt[node->get_child(i)] + 1;
            }
        }
    });

    std::cout << cnt[0] << "\n";
}

void solve_sam_link() {
    std::string s;
    std::cin >> s;

    OY::StaticSAM_string<26> sam(s.size(), [&](uint32_t i) { return s[i] - 'a'; });
    sam.prepare();

    ll ans = 0;
    sam.do_for_failing_nodes([&](uint32_t u) {
        auto node = sam.get_node(u);
        ans += node->m_length;
        if (int f = sam.query_fail(u); ~f) {
            ans -= sam.get_node(f)->m_length;
        }
    });

    std::cout << ans << "\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // solve_sa();
    // solve_sam();
    solve_sam_link();

    return 0;
}
#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"
#include "src/DataStruct/Fenwick/BIT_ex.h"
#include "src/DataStruct/AdjDiff.h"
#include "src/Tree/VectorTree.h"

using ll = long long;

/*
[CF587F Duff is Mad](https://codeforces.com/contest/587/problem/F)
[status](https://codeforces.com/contest/587/submission/329146316)
*/
/*
给定 n 个字符串 $$s_i$$，q 次询问，每次询问 $$s_{l...r}$$ 在 $$s_k$$ 中出现次数之和
$$1 \leq n, q, \sum |s_i| \leq 10^5$$


令 $$N = \sum |s_i|$$，根号分治：
1. 如果 $$|s_k| \leq B$$，按顺序扫过 $$n$$ 个模式串，将模式串的结尾，fail 子树的权值全部 +1，对于每个询问，查询 $$s_k$$ 的每个前缀结点的权值之和即可，使用树状数组维护。
复杂度为 $$O(q\log q + n \log N + qB \log N)$$

2. 如果 $$|s_k| > B$$，这样的询问不超过 $$\frac{N}{B}$$ 个，可以线性处理：先将 $$s_k$$ 的每个前缀结点权值设置为 1，每个模式串的贡献就是子树和，每个询问可以用前缀和作差得到。
复杂度为 $$O(\frac{N^2}{B})$$

当 $$B = \frac{N}{\sqrt{q \log N}}$$ 时，复杂度最优
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<std::string> a(n);
    for (auto &s : a) {
        std::cin >> s;
    }

    OY::ACAM<26> ac;
    const int N = 100000;
    ac.reserve(N);
    std::vector<int> pos(n);
    for (int i = 0; i < n; i++) {
        pos[i] = ac.insert_lower(a[i]);
    }
    ac.prepare();

    const int B = N / std::sqrt(q * std::log(N));

    std::vector<std::array<int, 4>> small_query(n);
    std::vector<std::vector<std::array<int, 3>>> large_query(n);
    for (int i = 0; i < q; i++) {
        int l, r, k;
        std::cin >> l >> r >> k;
        l--, k--;
        
        if (a[k].size() <= B) {
            small_query.push_back({l, k, -1, i});
            small_query.push_back({r, k, 1, i});
        } else {
            large_query[k].push_back({l, r, i});
        }
    }

    std::vector<ll> ans(q);
    auto solve_large = [&]() {
        for (int k = 0; k < n; k++) {
            if (large_query[k].empty()) continue;

            std::vector<int> val(ac.m_data.size());
            for (int i = 0, p = 0; i < a[k].size(); i++) {
                p = ac.next(p, a[k][i] - 'a');
                val[p]++;
            }
            ac.do_for_failing_nodes([&](int u) { val[ac.query_fail(u)] += val[u]; });
            OY::AdjSumTable<ll> table(n, [&](int i) { return val[pos[i]]; });
            for (auto [l, r, i] : large_query[k]) {
                ans[i] = table.query(l, r);
            }
        }
    };
    auto solve_small = [&]() {
        OY::VectorTree::Tree<bool> fail_tree(ac.m_data.size());
        ac.do_for_failing_nodes([&](int u) { fail_tree.add_edge(ac.query_fail(u), u); });

        int cur = 0;
        std::vector<int> sz(ac.m_data.size(), 1), dfn(ac.m_data.size());
        fail_tree.tree_dp_vertex(0, [&](int u, int f) { dfn[u] = cur++; }, 
            [&](int u, int v) { sz[u] += sz[v]; }, {});

        OY::VectorBIT_ex<ll> fen(ac.m_data.size());
        auto get = [&](const std::string &s) {
            ll res = 0;
            for (int i = 0, p = 0; i < s.size(); i++) {
                p = ac.next(p, s[i] - 'a');
                res += fen.query(dfn[p]);
            }
            return res;
        };
        
        std::sort(small_query.begin(), small_query.end());
        for (int i = 0, j = 0; i <= n; i++) {
            while (j < small_query.size() && i == small_query[j][0]) {
                auto [l, k, f, idx] = small_query[j];
                ans[idx] += f * get(a[k]);
                j++;
            }
            if (i < n) {
                fen.add(dfn[pos[i]], dfn[pos[i]] + sz[pos[i]], 1);
            }
        }
    };

    solve_large();
    solve_small();

    for (int i = 0; i < q; i++) {
        std::cout << ans[i] << '\n';
    }

    return 0;
}
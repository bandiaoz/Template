#include <bits/stdc++.h>
#include "src/Graph/shortest-path/Dijkstra_heap.h"

using ll = long long;

/*
[Shortest Path](https://judge.yosupo.jp/problem/shortest_path)(https://github.com/yosupo06/library-checker-problems/issues/173)
[status](https://judge.yosupo.jp/submission/247167)
*/
/**
 * 本题为最短路模板题
 */

struct sum_path {
    ll m_val;
    int m_step;
    bool operator<(const sum_path &rhs) const { return m_val < rhs.m_val; }
    bool operator==(const sum_path &rhs) const { return m_val == rhs.m_val; }
    sum_path operator+(uint32_t x) const { return {m_val + x, m_step + 1}; }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, s, t;
    std::cin >> n >> m >> s >> t;
    OY::DijkstraHeap::Graph<int> G(n, m);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        std::cin >> u >> v >> w;
        G.add_edge(u, v, w);
    }

    const ll inf_dist = 1e18;
    const int inf_step = 1e9;
    struct monoid {
        using value_type = int;
        using sum_type = sum_path;
        using compare_type = std::less<>;
        static sum_type op(const sum_type &x, value_type y) { return x + y; }
        static sum_type identity() { return {}; }
        static sum_type infinite() { return {inf_dist, inf_step}; }
    };

    auto sol = G.calc<monoid, void, true>(s, t);
    if (sol.query(t) == sol.infinite()) {
        std::cout << "-1\n";
    } else {
        std::cout << sol.query(t).m_val << ' ' << sol.query(t).m_step << '\n';
        sol.trace(t, [&](int from, int to) {
            std::cout << from << ' ' << to << '\n';
        });
    }

    return 0;
}
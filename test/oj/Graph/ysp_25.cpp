#include <bits/stdc++.h>
#include "src/Graph/EulerPath_udg.h"
#include "src/Graph/EulerPath_dg.h"

/*
[Eulerian Trail (Directed)](https://judge.yosupo.jp/problem/eulerian_trail_directed)(https://github.com/yosupo06/library-checker-problems/issues/25)
[status](https://judge.yosupo.jp/submission/244474)
*/
/**
 * 本题为有向图欧拉回路模板题
 */
/*
[Eulerian Trail (Undirected)](https://judge.yosupo.jp/problem/eulerian_trail_undirected)(https://github.com/yosupo06/library-checker-problems/issues/25)
[status](https://judge.yosupo.jp/submission/244470)
*/
/**
 * 本题为无向图欧拉路径模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;
    while (t--) {
        int n, m;
        std::cin >> n >> m;
        // OY::EulerPathUDG::Graph G(n, m);
        OY::EulerPathDG::Graph G(n, m);
        for (int i = 0; i < m; i++) {
            int u, v;
            std::cin >> u >> v;
            G.add_edge(u, v);
        }

        auto sol = G.calc();
        auto source = sol.get_source();
        if (source == -1) {
            std::cout << "No\n";
        } else {
            std::cout << "Yes\n";
            std::cout << sol.get_source();
            sol.trace([&](int index, int from, int to) {
                std::cout << ' ' << to;
            });
            std::cout << '\n';
            
            sol.trace([&](int index, int from, int to) {
                std::cout << index << ' ';
            });
            std::cout << '\n';
        }
    }

    return 0;
}
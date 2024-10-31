#include <bits/stdc++.h>
#include "src/Graph/EulerPath_dg.h"
#ifdef LOCAL
#include "pprint.hpp"
#else
#define debug(...)
#endif

using ll = long long;

/*
[P7771 【模板】欧拉路径](https://www.luogu.com.cn/problem/P7771)
[status](https://www.luogu.com.cn/record/186040124)
*/
/*
求有向图字典序最小的欧拉路径。
由于题目特殊要求，必须要把边排序。
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    std::vector<std::pair<int, int>> edges(m);
    for (auto &[u, v] : edges) {
        std::cin >> u >> v;
        u--, v--;
    }

    std::sort(edges.begin(), edges.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.second < rhs.second;
    });

    OY::EulerPathDG::Graph G(n, m);
    for (auto &[u, v] : edges) {
        G.add_edge(u, v);
    }

    auto sol = G.calc();
    int source = sol.get_source();
    if (source == -1) {
        std::cout << "No\n";
    } else {
        std::cout << source + 1;
        sol.trace([&](int index, int from, int to) {
            std::cout << ' ' << to + 1;
        });
    }

    return 0;
}
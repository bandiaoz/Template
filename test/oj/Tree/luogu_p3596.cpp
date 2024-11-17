#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/Tree/RaySeg.h"

using ll = long long;

/*
[P3596 [POI2015] MOD](https://www.luogu.com.cn/problem/P3596)
[status](https://www.luogu.com.cn/record/184878849)
*/
/**
 * 删去一条边，并添加一条边，求新树的最大直径和最小直径
 * 本题为射线线段助手模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::VectorTree::Tree<bool> tree(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        tree.add_edge(u, v);
    }

    tree.prepare();
    tree.set_root(0);

    OY::RAYSEG::Table<decltype(tree), int, 500000> rayseg(&tree);
    int min_ans = n, max_ans = 0;
    int min_up_vertex, min_down_vertex;
    int max_up_vertex, max_down_vertex;

    // dfs，在 dfs 的过程中尝试拆掉每条边，然后组装新的树
    auto dfs = [&](auto &&self, int u, int f) -> void {
        tree.do_for_each_adj_vertex(u, [&](int v) {
            if (v == f) return;
            auto down = rayseg.query_down_seg(v);
            auto up = rayseg.max_ray_seg_except_son(u, v, 1).m_seg;
            // 组装新树，令直径尽量小，那么就要连接两部分的直径的中点
            int min = std::max({down, up, (down + 1) / 2 + (up + 1) / 2 + 1});
            if (min <= min_ans) {
                min_ans = min;
                min_up_vertex = u;
                min_down_vertex = v;
            }
            // 组装新树，令直径尽量大，那么就要连接两部分的直径的端点
            int max = down + up + 1;
            if (max >= max_ans) {
                max_ans = max;
                max_up_vertex = u;
                max_down_vertex = v;
            }
            self(self, v, u);
        });
    };
    dfs(dfs, 0, -1);

    auto get_diameter_middle = [&](int source, int ban) {
        auto filter = [&](int a) { return a != ban; };
        std::vector<int> path;
        auto call = [&](int u) { path.push_back(u); };
        OY::RAYSEG::Diameter<decltype(tree), int, 500000>::solve(&tree, source, filter, call);
        return path[path.size() / 2];
    };

    // 求出分割后，上侧和下侧的直径中点
    int min_up_middle = get_diameter_middle(min_up_vertex, min_down_vertex);
    int min_down_middle = get_diameter_middle(min_down_vertex, min_up_vertex);
    std::cout << min_ans << ' ' << min_up_vertex + 1 << ' ' << min_down_vertex + 1 << ' ' 
        << min_up_middle + 1 << ' ' << min_down_middle + 1 << '\n';

    auto get_diameter_end = [&](int source, int ban) {
        auto filter = [&](int a) { return a != ban; };
        std::vector<int> path;
        auto call = [&](int u) { path.push_back(u); };
        OY::RAYSEG::Diameter<decltype(tree), int, 500000>::solve(&tree, source, filter, call);
        return path.back();
    };

    // 求出分割后，上侧和下侧的直径端点
    int max_up_end = get_diameter_end(max_up_vertex, max_down_vertex);
    int max_down_end = get_diameter_end(max_down_vertex, max_up_vertex);
    std::cout << max_ans << ' ' << max_up_vertex + 1 << ' ' << max_down_vertex + 1 << ' ' 
        << max_up_end + 1 << ' ' << max_down_end + 1 << '\n';

    return 0;
}
#include <bits/stdc++.h>
#include "src/Graph/flow/Dinic.h"

/*
[CF510E Fox And Dinner](https://codeforces.com/contest/510/problem/E)
[status](https://codeforces.com/contest/510/submission/340877131)
*/
/*
有 $$n(3 \leq n \leq 200)$$ 只狐狸，年龄为 $$a_i(2 \leq a_i \leq 10^4)$$，分配到若干圆桌上，要求:
1. 每只狐狸至少在一张桌子上
2. 每桌至少 3 只狐狸
3. 相邻狐狸年龄之和为质数
求分配方案

圆桌一定是奇数偶数相邻，那么环的大小一定是偶数，构建二分图，左部为奇数，右部为偶数
source 向左部连边，容量为 2，右部向 sink 连边，容量为 2
如果左部和右部之和为质数，则连边，容量为 1
跑出最大流，如果最大流不等于 n，则无解，否则流量为 1 的边就是答案
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    auto is_prime = [&](int x) {
        for (int i = 2; i * i <= x; i++) {
            if (x % i == 0) {
                return false;
            }
        }
        return true;
    };
    const int source = n, sink = n + 1;
    OY::DINIC::Graph<int> flow(n + 2);
    for (int i = 0; i < n; i++) {
        if (a[i] % 2 == 0) {
            flow.add_edge(source, i, 2);
        } else {
            flow.add_edge(i, sink, 2);
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i] % 2 == 0 && a[j] % 2 == 1 && is_prime(a[i] + a[j])) {
                flow.add_edge(i, j, 1);
            }
        }
    } 

    if (flow.calc(source, sink) != n) {
        std::cout << "Impossible\n";
    } else { 
        std::vector<std::vector<int>> g(n);
        flow.do_for_flows([&](int i, int f) {
            auto [u, v, cap] = flow.m_raw_edges[i];
            if (f == 1) {
                g[u].push_back(v);
                g[v].push_back(u);
            }
        });

        std::vector<int> vis(n);
        std::vector<std::vector<int>> ans;
        for (int i = 0; i < n; i++) {
            if (vis[i]) continue;
            std::vector<int> path;
            auto dfs = [&](auto &&self, int u) -> void {
                vis[u] = true;
                path.push_back(u);
                for (int v : g[u]) {
                    if (vis[v]) continue;
                    self(self, v);
                }
            };
            dfs(dfs, i);
            ans.push_back(path);
        }

        std::cout << ans.size() << "\n";
        for (auto &path : ans) {
            std::cout << path.size() << " ";
            for (int i = 0; i < path.size(); i++) {
                std::cout << path[i] + 1 << " \n"[i == path.size() - 1];
            }
        }
    }

    return 0;
}
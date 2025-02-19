#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

/*
[P4819 [中山市选] 杀人游戏](https://www.luogu.com.cn/problem/P4819)
[status](https://www.luogu.com.cn/record/203216920)
*/
/**
 * 有 n 个人，其中有一个杀手，警察可以进行查证：
 * 如果查证的人是平民，会告诉警察所有认识的人的身份；否则警察会死。
 * 最优情况下，警察查证出杀手的概率是多少；
 * 
 * 先缩点获得一个 DAG，查证所有入度为 0 的连通分量可以就可以知道所有人的身份。
 * 假设有 c 个入度为 0 的连通分量，需要查证 c 个人，有凶手的概率是 c / n，警察成功概率就是 1 - c / n。
 * 但是如果存在一个入度为 0 的连通分量，且大小为 1，且他认识的人都有其他人认识，那么可以不用查证这个人，就知道其他 n-1 人的身份。
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::SCC::Graph graph(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
    }

    auto scc = graph.calc();
    auto groups = scc.get_groups();

    std::vector<int> dangers(groups.size(), true);
    std::vector<int> deg(groups.size());
    std::vector<int> last(groups.size(), -1); // 记录最后连向这个连通分量的编号，保证统计度数不计算重边
    int cnt = 0; // 需要查证的人数
    for (int i = 0; i < groups.size(); i++) {
        for (auto u : groups[i]) {
            graph(u, [&](int v) {
                if (int j = scc.query(v); j != i && last[j] != i) {
                    deg[j]++;
                    last[j] = i;
                }
            });
        }
        if (deg[i] == 0) {
            cnt++;
        }
    }

    for (int i = 0; i < groups.size(); i++) {
        if (deg[i] == 0 && groups[i].size() == 1) {
            bool flag = true;
            for (auto u : groups[i]) {
                graph(u, [&](int v) {
                    if (deg[scc.query(v)] == 1) {
                        flag = false;
                    }
                });
            }
            if (flag) {
                cnt--;
                break;
            }
        }
    }

    std::cout << std::fixed << std::setprecision(6) << 1. * (n - cnt) / n << "\n";

    return 0;
}
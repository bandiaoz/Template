#include <bits/stdc++.h>

using ll = long long;

/*
[B. MADMAX](https://codeforces.com/contest/917/problem/B)
[status](https://codeforces.com/contest/917/submission/301329862)
*/
/**
 * 两个人在有向图上博弈，边上有字符，先手起点在 A，后手起点在 B
 * 轮流行动，并且两个人每次经过的边上的字符必须不降
 * 对于 n*n 种起点，谁能必胜
 * 
 * 使用判定引理，如果从 (x, y) 出发，可以到达一个必败局面 (y, v)，其中 x->v，则 (x, y) 必胜
 * sg 的状态为 sg[x][y][cur]，表示起点为 (x, y) 出发，只能走大于等于 cur 的边
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<std::pair<int, int>>> g(n);
    while (m--) {
        int u, v;
        char c;
        std::cin >> u >> v >> c;
        u--, v--;
        g[u].emplace_back(v, c - 'a');
    }

    std::vector sg(n, std::vector(n, std::vector<int>(26, -1)));
    auto dfs = [&](auto &&self, int x, int y, int cur) -> bool {   
        if (sg[x][y][cur] != -1) {
            return sg[x][y][cur];
        }
        
        sg[x][y][cur] = 0;
        for (auto [v, c] : g[x]) {
            if (c >= cur && !self(self, y, v, c)) {
                sg[x][y][cur] = 1;
                break;
            }
        }

        return sg[x][y][cur];
    };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << (dfs(dfs, i, j, 0) ? 'A' : 'B');
        }
        std::cout << '\n';
    }

    return 0;
}
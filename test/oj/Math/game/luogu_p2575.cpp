#include <bits/stdc++.h>

/*
[P2575 高手过招](https://www.luogu.com.cn/problem/P2575)
[status](https://www.luogu.com.cn/record/198730649)
*/
/**
 * 有 n 行 20 列的棋盘，对于一个棋子，可以向右移动到第一个空格，如果不能移动则输掉比赛
 * 问先手能否获胜
 * 
 * 多个有向图的 sg 为所有有向图的 sg 值的异或和
 * 多个后继状态的 sg 为所有后继状态的 sg 值的 mex
 * 
 * 相当于 n 个有向图游戏，必败局面 sg = 0
 * sg(x) = mex{sg(y) | x -> y}
 * 对于每一行 2^{20} 个状态，求出 sg 值，所有行 sg 值的异或和为所有游戏的 sg 值，sg 值为 0 则先手必败
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    constexpr int N = 20;
    std::vector<int> sg(1 << N, -1);
    auto dfs = [&](auto &&self, int mask) -> int {
        if (sg[mask] != -1) return sg[mask];

        int last = -1;
        std::vector<int> vis(N);
        for (int i = N - 1; i >= 0; i--) {
            if (mask >> i & 1) {
                if (last != -1) {
                    int next_sg = self(self, mask ^ (1 << last) ^ (1 << i));
                    vis[next_sg] = 1;
                }
            } else {
                last = i;
            }
        }

        for (int i = 0; i < N; i++) {
            if (!vis[i]) {
                sg[mask] = i;
                break;
            }
        }

        return sg[mask];
    };
    for (int mask = 0; mask < (1 << N); mask++) {
        dfs(dfs, mask);
    }

    while (t--) {
        int n;
        std::cin >> n;

        int ans = 0;
        for (int i = 0; i < n; i++) {
            int m, mask = 0;
            std::cin >> m;
            for (int j = 0; j < m; j++) {
                int x;
                std::cin >> x;
                mask |= 1 << (x - 1);
            }
            ans ^= sg[mask];
        }

        std::cout << (ans ? "YES" : "NO") << '\n';
    }

    return 0;
}
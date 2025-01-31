#include <bits/stdc++.h>

/*
[235. 魔法珠](https://www.acwing.com/problem/content/description/237/)
[status](https://www.acwing.com/problem/content/submission/code_detail/38829166/)
*/
/**
 * 给定 n 个数，每次操作可以一个数 x > 1，将 x 替换为所有小于 x 的因子，并删除其中一个因子
 * 问先手能否获胜
 * 
 * x 的因子为 f_1, f_2, ..., f_k，
 * 令 sum = sg(f_1) ^ sg(f_2) ^ ... ^ sg(f_k)
 * 则 x 的一共有 k 个后继状态(x -> y)，分别为 sg(y) = sum ^ sg(f_i)
 * 
 * sg(x) = mex{sg(y) | x -> y}
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr int N = 1000;
    std::vector<int> sg(N + 1, -1);
    auto dfs = [&](auto &&self, int x) -> int {
        if (sg[x] != -1) return sg[x];
        if (x == 1) return sg[x] = 0;

        std::vector<int> factor{1};
        for (int i = 2; i * i <= x; i++) {
            if (x % i == 0) {
                factor.push_back(i);
                if (i * i != x) factor.push_back(x / i);
            }
        }

        int sum = std::accumulate(factor.begin(), factor.end(), 0, [&](int s, int x) {
            return s ^ self(self, x);
        });

        std::vector<int> vis(200);
        for (auto i : factor) {
            int next_sg = sum ^ self(self, i);
            vis[next_sg] = 1;
        }

        for (int i = 0; i < vis.size(); i++) {
            if (!vis[i]) {
                sg[x] = i;
                break;
            }
        }
        return sg[x];
    };

    int n;
    while (std::cin >> n) {
        int ans = 0;
        for (int i = 0; i < n; i++) {
            int x;
            std::cin >> x;
            ans ^= dfs(dfs, x);
        }
        std::cout << (ans ? "freda" : "rainbow") << '\n';
    }

    return 0;
}
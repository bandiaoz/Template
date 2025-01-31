#include <bits/stdc++.h>

/*
[219. 剪纸游戏](https://www.acwing.com/problem/content/221/)
[status](https://www.acwing.com/problem/content/submission/code_detail/38828253/)
*/
/**
 * 有大小为 n*m 的纸，每次可以沿着一条直线剪开，例如 (n, m) -> (x, m) 和 (n - x, m)
 * 剪出 1*1 即可获胜，问先手能否获胜。
 * 
 * 有向图游戏的终点是没有任何后继状态，而剪出 1*1 之后，还可以选择其他的纸继续剪，因此不可终止条件
 * 
 * 将规则等价为，不允许剪出 1*m 或者 n*1 的纸，这样无法操作则失败，符合有向图游戏概念：
 * 多个有向图的 sg 为所有有向图的 sg 值的异或和
 * 多个后继状态的 sg 为所有后继状态的 sg 值的 mex
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const int N = 200;
    std::vector sg(N + 1, std::vector(N + 1, -1));
    auto dfs = [&](auto &&self, int n, int m) -> int {
        if (sg[n][m] != -1) return sg[n][m];

        std::vector<int> vis(2 * N + 1);
        for (int i = 2; i + 2 <= n; i++) {
            int a = self(self, i, m);
            int b = self(self, n - i, m);
            vis[a ^ b] = 1;
        }
        for (int i = 2; i + 2 <= m; i++) {
            int a = self(self, n, i);
            int b = self(self, n, m - i);
            vis[a ^ b] = 1;
        }
        for (int i = 0; i < vis.size(); i++) {
            if (!vis[i]) return sg[n][m] = i;
        }
        return sg[n][m] = vis.size();
    };
    
    int n, m;
    while (std::cin >> n >> m) {
        std::cout << (dfs(dfs, n, m) ? "WIN" : "LOSE") << '\n';
    }

    return 0;
}
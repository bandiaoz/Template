#include <bits/stdc++.h>

/*
[P1247 取火柴游戏](https://www.luogu.com.cn/problem/P1247)
[status](https://www.luogu.com.cn/record/198637162)
*/
/**
 * 单个有向图游戏，nim 游戏，需要输出方案
 * 判定引理：
 * 1. 游戏的终点即所有火柴数均为 0 时，先手必败
 * 2. 非终点时，必胜局面当且仅当可以到达一个必败局面
 * 3. 非终点时，必败局面当且仅当无法到达任意一个必败局面
 * 
 * sg 引理：
 * 对于状态 x 和所有的后继状态 y，有 sg(x) = mex{sg(y) | x -> y}
 * sg 引理能够刻画三条判定引理的性质
 * 
 * 在本题中，sg 的值为所有火柴数的异或和，如果 a[i] 可以减小到 a[i] ^ sg 则 a[i] 必胜
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

    int sg = std::accumulate(a.begin(), a.end(), 0, std::bit_xor());

    if (sg == 0) {
        std::cout << "lose\n";
    } else {
        for (int i = 0; i < n; i++) {
            if ((a[i] ^ sg) < a[i]) {
                std::cout << (a[i] - (a[i] ^ sg)) << ' ' << i + 1 << '\n';
                a[i] = a[i] ^ sg;
                break;
            }
        }
        for (int i = 0; i < n; i++) {
            std::cout << a[i] << " \n"[i == n - 1];
        }
    }

    return 0;
}
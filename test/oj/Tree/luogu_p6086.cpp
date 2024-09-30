#include <bits/stdc++.h>
#include "src/Tree/Prufer.h"

using ll = long long;

/*
[P6086 【模板】Prufer 序列](https://www.luogu.com.cn/problem/P6086)
[status](https://www.luogu.com.cn/record/177264211)
*/
/**
 * 本题为 Prufer 模板题，有编码和解码两个任务
 */
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, type;
    std::cin >> n >> type;

    if (type == 1) {
        OY::PRUFER::Tree g(n);
        for (int i = 0; i < n - 1; i++) {
            int f;
            std::cin >> f;
            f--, 
            g.add_edge(f, i);
        }

        auto code = g.encode();
        ll ans = 0;
        for (int i = 0; i < n - 2; i++) {
            ans ^= 1LL * (i + 1) * (code[i] + 1);
        }
        std::cout << ans << '\n';
    } else {
        std::vector<int> code(n - 2);
        for (auto &x : code) {
            std::cin >> x;
            x--;
        }

        auto fa = OY::PRUFER::Tree::decode(code.begin(), code.end());
        ll ans = 0;
        for (int i = 0; i < n - 1; i++) {
            ans ^= 1LL * (i + 1) * (fa[i] + 1);
        }
        std::cout << ans << '\n';
    }

    return 0;
}
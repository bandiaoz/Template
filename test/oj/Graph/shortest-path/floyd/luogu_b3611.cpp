#include <bits/stdc++.h>
#include "src/Graph/shortest-path/FloydWarshall.h"

/*
[B3611 【模板】传递闭包](https://www.luogu.com.cn/problem/B3611)
[status](https://www.luogu.com.cn/record/180381662)
*/
/**
 * 本题为传递闭包模板题，需要用到 FloydWarshall 算法
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::FloydWarshall::Graph<100> G(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int x;
            std::cin >> x;
            if (x) {
                G.add_edge(i, j);
            }
        }
    }

    G.calc();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << G.can(i, j) << " \n"[j == n - 1];
        }
    }

    return 0;
}
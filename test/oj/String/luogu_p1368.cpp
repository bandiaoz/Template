#include <bits/stdc++.h>
#include "src/String/MinimalSequence.h"

/*
[P1368 【模板】最小表示法](https://www.luogu.com.cn/problem/P1368)
[status](https://www.luogu.com.cn/record/184224690)
*/
/**
 * 最小表示法标准做法可以有 O(n) 的小常数做法
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

    auto start = OY::get_minimal_rotation(a.begin(), a.end());
    for (int i = 0; i < n; i++) {
        std::cout << a[(start + i) % n] << " \n"[i == n - 1];
    }

    return 0;
}
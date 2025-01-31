#include <bits/stdc++.h>
#include "src/DataStruct/BalancedTree/AVL.h"

/*
[P6136 【模板】普通平衡树（数据加强版）](https://www.luogu.com.cn/problem/P6136)
[status(avl)](https://www.luogu.com.cn/record/198405199)
*/
/**
 * 本题为 P3369 加强版
 * 为二叉平衡树模板题
 */

using AVL = OY::AVLMultiset<int>;

void solve_avl() {
    int n, m;
    std::cin >> n >> m;

    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }
    std::sort(a.begin(), a.end());

    AVL::_reserve(n);
    auto avl = AVL::from_sorted(a.begin(), a.end());
    int last = 0, sum = 0;
    while (m--) {
        int op, x;
        std::cin >> op >> x;
        x ^= last;

        if (op == 1) {
            avl.insert_by_key(x);
        } else if (op == 2) {
            avl.erase_by_key(x);
        } else if (op == 3) {
            last = avl.rank(x) + 1;
            sum ^= last;
        } else if (op == 4) {
            last = avl.kth(x - 1)->get();
            sum ^= last;
        } else if (op == 5) {
            last = avl.smaller_bound(x)->get();
            sum ^= last;
        } else {
            last = avl.upper_bound(x)->get();
            sum ^= last;
        }
    }

    std::cout << sum << "\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_avl();

    return 0;
}
#include <bits/stdc++.h>
#include "src/DataStruct/BalancedTree/AVL.h"

/*
[P3369 【模板】普通平衡树](https://www.luogu.com.cn/problem/P3369)
[status(avl)](https://www.luogu.com.cn/record/198403638)
*/
/**
 * 本题要维护一个有序集合，查询元素名次，根据名次查询元素
 * 为普通平衡树模板题
 */

void solve_avl() {
    int n;
    std::cin >> n;
    OY::AVLMultiset<int, std::less<int>>::_reserve(n);
    OY::AVLMultiset<int, std::less<int>> avl;
    for (int i = 0; i < n; i++) {
        int op, x;
        std::cin >> op >> x;

        if (op == 1) {
            avl.insert_by_key(x);
        } else if (op == 2) {
            avl.erase_by_key(x);
        } else if (op == 3) {
            std::cout << avl.rank(x) + 1 << "\n";
        } else if (op == 4) {
            std::cout << avl.kth(x - 1)->get() << "\n";
        } else if (op == 5) {
            std::cout << avl.smaller_bound(x)->get() << "\n";
        } else {
            std::cout << avl.upper_bound(x)->get() << "\n";
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_avl();

    return 0;
}
#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/ChthollyZkwTree.h"
#include "src/DataStruct/Segtree/ChthollySegTree.h"

/*
[P4979 矿洞：坍塌](https://www.luogu.com.cn/problem/P4979)
[status(ChthollyZkwTree)](https://www.luogu.com.cn/record/196794035)
[status(ChthollySegTree)](https://www.luogu.com.cn/record/196811156)
*/
/**
 * 给定长度为 n 的字符串，字符集为 'A', 'B', 'C'
 * 有两种操作：
 * 1. 将区间 [l, r] 替换为 x
 * 2. 检查区间 [l, r] 是否是一个合法的区间，即区间内字符都相同，并且 l-1 和 r+1 处的字符不同
 * 
 * 本题可以使用珂朵莉树、RangeManager 等数据结构解决
 */

void solve_czkw() {
    int n, m;
    std::string s;
    std::cin >> n >> s >> m;

    OY::ChthollyZkwTree<char> cho(n, 'A');
    // OY::VectorChthollySegTree<char> cho(n, 'A');
    for (int i = 0; i < n; i++) {
        if (s[i] != 'A') cho.modify(i, s[i]);
    }

    while (m--) {
        char op;
        std::cin >> op;

        if (op == 'A') {
            int l, r;
            char ch;
            std::cin >> l >> r >> ch;
            l--;
            cho.modify(l, r, ch);
        } else {
            int l, r;
            std::cin >> l >> r;
            l--;
            
            if (cho.query(l, r).has_change()) {
                std::cout << "No\n";
            } else if (l - 1 >= 0 && r + 1 <= n && cho.query(l - 1) == cho.query(r)) {
                std::cout << "No\n";
            } else {
                std::cout << "Yes\n";
            }
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_czkw();

    return 0;
}
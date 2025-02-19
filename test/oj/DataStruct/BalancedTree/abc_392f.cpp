#include <bits/stdc++.h>
#include "src/DataStruct/BalancedTree/MonoAVL.h"

/*
[F - Insert](https://atcoder.jp/contests/abc392/tasks/abc392_f)
[status](https://atcoder.jp/contests/abc392/submissions/62631073)
*/
/**
 * 对于一个空数组，分别将 1, 2, 3, ... 插入到位置 p_i 上，求最终数组
 * 
 * 等价于初始化数组 [1, 2, 3, ...]，每次插入操作相当于做两次区间翻转，使用 AVL 树维护
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    using Tree = OY::MonoAVLSequence<int, true>;
    Tree::_reserve(n + 1);
    auto tree = Tree::from_mapping(n, [](int i) { return i + 1; });
    using node = decltype(tree)::node;

    for (int i = 0; i < n; i++) {
        int p;
        std::cin >> p;
        p--;

        tree.reverse(p, i + 1);
        tree.reverse(p + 1, i + 1);
    }

    tree.enumerate([](node *p) { std::cout << p->m_val << ' '; });

    return 0;
}
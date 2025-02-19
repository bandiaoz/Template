#include <bits/stdc++.h>
#include "src/DataStruct/BalancedTree/MonoAVL.h"

/*
[P3391 【模板】文艺平衡树](https://www.luogu.com.cn/problem/P3391)
[status](https://www.luogu.com.cn/record/202086237)
*/
/**
 * 数组初始 a[i] = i，进行若干次区间翻转之后，输出最终的数组
 * 
 * 本题需要一种支持分裂、合并、区间修改的数据结构
 * 线段树支持区间修改，但是线段树的形态是定死的，不能进行左右孩子交换的修改
 * 只能选择二叉平衡树
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    using Tree = OY::MonoAVLSequence<int, true>;
    Tree::_reserve(n + 1);
    auto tree = Tree::from_mapping(n, [](int i) { return i + 1; });
    using node = decltype(tree)::node;

    while (m--) {
        int l, r;
        std::cin >> l >> r;
        l--;
        tree.reverse(l, r);
    }

    tree.enumerate([](node *p) {
        std::cout << p->m_val << ' ';
    });

    return 0;
}
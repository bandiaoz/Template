#include <bits/stdc++.h>
#include "src/String/KMP.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"

/*
[P5829 【模板】失配树](https://www.luogu.com.cn/problem/P5829)
[status](https://www.luogu.com.cn/record/196681403)
*/
/**
 * 给定一个字符串 s 和 m 次询问，每次询问长度为 p 的前缀和长度为 q 的前缀的最长公共 border 长度
 * 
 * 失配树模板题
 * 长度为 v 的前缀的 border 长度为 u，则 u 向 v 连边，得到失配树
 * 这样，每个节点的所有祖先都是它的 border
 * 长度为 p 的前缀和长度为 q 的前缀的最长公共 border 长度就是它们在失配树上的最近公共祖先
 * 注意，如果 lca 是 p 或 q，则答案是 lca 的父节点，因为 border 不包括自身
 * 也可以直接用 lca(fa(p), fa(q)) 来求解
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;
    int n = s.size();

    OY::KMP_string kmp(s);
    OY::VectorTree::Tree<bool> tree(n + 1);
    for (int i = 1; i <= n; i++) {
        tree.add_edge(kmp.query_Pi(i - 1), i);
    }

    tree.set_root(0);
    tree.prepare();

    OY::HLD::Table<decltype(tree)> hld(&tree);

    int m;
    std::cin >> m;
    while (m--) {
        int p, q;
        std::cin >> p >> q;
        std::cout << hld.lca(kmp.query_Pi(p - 1), kmp.query_Pi(q - 1)) << "\n";
    }

    return 0;
}
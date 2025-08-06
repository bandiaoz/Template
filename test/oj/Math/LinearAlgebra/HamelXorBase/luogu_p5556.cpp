#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBase.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HLDZkw.h"

/*
[P5556 圣剑护符](https://www.luogu.com.cn/problem/P5556)
[status](https://www.luogu.com.cn/record/208210134)
*/
/**
 * 一棵树，每个点有点权，有 q 次操作：
 * 1. 修改一个点的点权
 * 2. 查询编号 x 和 y 的简单路径集合，是否存在两个不相等的子集（子集可以为空），使得两个子集异或和相等。
 * 
 * 如果有两个非空子集异或和相等，则两个子集异或和等于 0，等于空集的异或和。所以只需要查询是否存在一个非空子集异或和为 0 即可。
 * 暴力插入路径上的点权，如果插入失败，则输出 YES。线性基的大小（基向量数量）最多为 $$\omega = 30$$，超过 $$\omega$$ 则一定存在一个非空子集异或和为 0。
 * 
 * 修改的复杂度为树剖+线段树，$$O(\log^2 n)$$
 * 查询的复杂度为 $$O(\omega^2)$$，路径长度最多为 $$\omega = 30$$，单次插入复杂度为 $$O(\omega)$$
 */

using Info = int;
using Tag = int;
auto op = std::bit_xor<Info>();
auto map = [](Tag new_modify, Info old_val, uint32_t len) {
    return new_modify ^ old_val;
};
auto com = [](Tag new_modify, Tag old_modify) {
    return new_modify ^ old_modify;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    OY::VectorTree::Tree<bool> tree(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        tree.add_edge(u, v);
    }

    tree.set_root(0);
    tree.prepare();

    auto mapping = [&](int u) { return a[u]; };
    auto zkw = OY::make_lazy_HLDZkw<Info, Tag, true>(&tree, mapping, op, map, com, 0);
    auto &hld = zkw.m_hld;
    auto dist = [&](int u, int v) {
        int lca = hld.lca(u, v);
        return hld.m_info[u].m_dep + hld.m_info[v].m_dep - 2 * hld.m_info[lca].m_dep;
    };

    while (q--) {
        std::string op;
        std::cin >> op;
        int u, v, z;
        std::cin >> u >> v;
        u--, v--;

        if (op == "Update") {
            std::cin >> z;
            zkw.add_path<true>(u, v, z);
        } else if (dist(u, v) > 30) {
            std::cout << "YES\n";
        } else {
            bool ok = false;
            OY::StaticHamelXorBase32<30> hxb;
            hld.do_for_path<true>(u, v, [&](int l, int r) {
                for (int i = l; i < r && !ok; i++) {
                    if (int u = hld.m_seq[i]; hxb.insert(zkw.query(u)) == -1) {
                        ok = true;
                    }
                }
            });
            std::cout << (ok ? "YES" : "NO") << '\n';
        }
    }

    return 0;
}
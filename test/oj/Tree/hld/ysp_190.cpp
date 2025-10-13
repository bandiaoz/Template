#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"
#include "src/DataStruct/Segtree/MonoZkwTree.h"
#include "src/Math/Modular/StaticModInt32.h"

using Z = OY::mint998244353;

/*
[Vertex Set Path Composite](https://judge.yosupo.jp/problem/vertex_set_path_composite)(https://github.com/yosupo06/library-checker-problems/issues/190)
[status](https://judge.yosupo.jp/submission/319093)
*/
/*
给定一棵树，每个点有 $f_i(x) = a_i x + b_i$，有 $$q$$ 次操作：
1. 将一个点的 $$f_i(x)$$ 修改为 $$cx + d$$
2. 查询一条路径上的 $$f_i(x)$$ 的复合，即 $$f_{p_k}(...f_{p_2}(f_{p_1}(x)))$$

使用 HLD + MonoZkwTree + directed_path 解决，注意需要开两棵树，一棵正向合并，一棵反向合并
*/

struct Info {
    Z mul, add;
    Z calc(Z x) const {
        return mul * x + add;
    }
};
constexpr Info identity{1, 0};
auto op = [](const Info &a, const Info &b) -> Info {
    return Info{a.mul * b.mul, a.add * b.mul + b.add};
};
auto op_reverse = [](const Info &a, const Info &b) -> Info {
    return Info{b.mul * a.mul, b.add * a.mul + a.add};
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<Info> a(n);
    for (auto &i : a) {
        std::cin >> i.mul >> i.add;
    }
    OY::VectorTree::Tree<bool> tree(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        tree.add_edge(u, v);
    }

    tree.set_root(0);
    tree.prepare();
    OY::HLD::Table<decltype(tree)> hld(&tree);

    auto zkw = OY::make_MonoZkw<Info, identity>(n, op, [&](uint32_t i) {
        return a[hld.m_seq[i]];
    });
    auto zkw_reverse = OY::make_MonoZkw<Info, identity>(n, op_reverse, [&](uint32_t i) {
        return a[hld.m_seq[i]];
    });

    while (q--) {
        int op;
        std::cin >> op;

        if (op == 0) {
            int p, c, d;
            std::cin >> p >> c >> d;
            zkw.modify(hld.m_info[p].m_dfn, Info{c, d});
            zkw_reverse.modify(hld.m_info[p].m_dfn, Info{c, d});
        } else {
            int u, v, x;
            std::cin >> u >> v >> x;
            Info res{1, 0};
            hld.do_for_directed_path(u, v, [&](uint32_t l, uint32_t r) {
                if (l < r) {
                    res = ::op(res, zkw.query(l, r));
                } else {
                    res = ::op(res, zkw_reverse.query(r, l));
                }
            });

            std::cout << res.calc(x) << '\n';
        }
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/DataStruct/DSU/PotentializedDSU.h"

/*
[Unionfind with Potential](https://judge.yosupo.jp/problem/unionfind_with_potential)(https://github.com/yosupo06/library-checker-problems/issues/1194)
[status](https://judge.yosupo.jp/submission/244682)
*/
/*
[Unionfind with Potential (Non-Commutative Group)](https://judge.yosupo.jp/problem/unionfind_with_potential_non_commutative_group)(https://github.com/yosupo06/library-checker-problems/issues/1194)
[status](https://judge.yosupo.jp/submission/244685)
*/
/**
 * 维护势能的并查集模板题
 * 维护每个点到自己父结点的势能差；压缩路径时做相应修改
 * 矩阵版本保证了输入的矩阵的行列式都为 1
 */

void main_commutative() {
    int n, q;
    std::cin >> n >> q;

    static constexpr int Mod = 998244353;
    OY::ModAddPDSUTable<int, Mod, true> dsu(n);

    while (q--) {
        int op;
        std::cin >> op;
        if (op == 0) {
            int u, v, dis;
            std::cin >> u >> v >> dis;
            if (dsu.unite_by_size(v, u, dis) || dsu.calc(v, u).second == dis) {
                std::cout << "1\n";
            } else {
                std::cout << "0\n";
            }
        } else {
            int u, v;
            std::cin >> u >> v;
            if (auto [exist, dis] = dsu.calc(v, u); exist) {
                std::cout << dis << '\n';
            } else {
                std::cout << "-1\n";
            }
        }
    }
}

void main_non_commutative() {
    int n, q;
    std::cin >> n >> q;

    static constexpr uint32_t Mod = 998244353;
    struct Group {
        using value_type = std::array<std::array<int, 2>, 2>;
        static value_type identity() { return {1, 0, 0, 1}; }
        static value_type op(value_type lhs, value_type rhs) { 
            value_type ans{};
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 2; k++) {
                        ans[i][j] = (ans[i][j] + 1LL * lhs[i][k] * rhs[k][j]) % Mod;
                    }
                }
            }
            return ans;
        }
        static value_type inverse(value_type x) { 
            return {
                x[1][1], x[0][1] ? Mod - x[0][1] : 0, 
                x[1][0] ? Mod - x[1][0] : 0, x[0][0]
            }; 
        }
    };
    using value_type = Group::value_type;
    OY::PDSU::Table<Group, true> dsu(n);

    while (q--) {
        int op;
        std::cin >> op;

        if (op == 0) {
            int u, v;
            value_type dis;
            std::cin >> u >> v;
            std::cin >> dis[0][0] >> dis[0][1] >> dis[1][0] >> dis[1][1];
            if (dsu.unite_by_size(v, u, dis) || dsu.calc(v, u).second == dis) {
                std::cout << "1\n";
            } else {
                std::cout << "0\n";
            }
        } else {
            int u, v;
            std::cin >> u >> v;
            if (auto [exist, dis] = dsu.calc(v, u); exist) {
                std::cout << dis[0][0] << ' ' << dis[0][1] << ' ';
                std::cout << dis[1][0] << ' ' << dis[1][1] << '\n';
            } else {
                std::cout << "-1\n";
            }
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // main_commutative();
    main_non_commutative();

    return 0;
}
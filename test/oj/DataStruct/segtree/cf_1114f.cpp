#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/ZkwTree.h"
#include "src/Math/Modular/StaticModInt32.h"
#include "src/Math/NumberTheory/EulerSieve.h"

using Z = OY::mint1000000007;

/*
[CF1114F Please, another Queries on Array?](https://codeforces.com/contest/1114/problem/F)
[status](https://codeforces.com/contest/1114/submission/308876918)
*/
/**
 * 有一个数组，有 q 次操作：
 * 1. 对于区间 [l, r] 中的每个元素乘 x
 * 2. 求 \phi(\prod_{i=l}^{r} a_i) mod 10^9 + 7
 * 
 * 根据 \gcd(x, y) = 1 -> \phi(x * y) = \phi(x) * \phi(y)
 * 则 \phi(x) = x * \prod_{p | x, p >= 2} ((p - 1) / p)
 * 线段树维护区间乘积和每个因子是否出现
 * 注意 tag 标记，区间每个数都要乘， map 操作需要对区间乘积乘上 power(tag, len)
 */

constexpr int N = 300;
OY::EulerSieve::Sieve<N, false, true, false> sieve;
std::vector<Z> phi_helper(N);
auto init_phi_helper = []() -> bool {
    for (int p = 2; p < N; p++) {
        phi_helper[p] = Z(p - 1) / p;
    }
    return true;
}();

struct Info {
    Z mul;
    std::bitset<N> cnt;
    Info() : mul(1), cnt() {}
    Info(Z mul, std::bitset<N> cnt) : mul(mul), cnt(cnt) {}
    Info(int x) : mul(x), cnt() {
        for (auto [p, c] : sieve.decomposite(x)) {
            cnt.set(p);
        }
    }
    Z get() {
        Z phi = mul;
        for (int p = 2; p < N; p++) {
            if (cnt[p]) {
                phi = phi * phi_helper[p];
            }
        }
        return phi;
    }
};
using Tag = Info;

auto op = [](Info x, Info y) -> Info { return Info{x.mul * y.mul, x.cnt | y.cnt}; };
auto map = [](Tag new_modify, Info old_val, uint32_t len) -> Info {
    return Info{
        new_modify.mul.pow(len) * old_val.mul,
        new_modify.cnt | old_val.cnt
    };
};
auto com = [](Tag new_modify, Tag old_modify) -> Tag { return op(new_modify, old_modify); };

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    auto mapping = [&](int i) { return Info(a[i]); };
    auto zkw = OY::make_lazy_ZkwTree<Info, Tag, true>(n, mapping, op, map, com, Tag());

    while (q--) {
        std::string op;
        int l, r, x;
        std::cin >> op >> l >> r;
        l--;

        if (op == "MULTIPLY") {
            std::cin >> x;
            zkw.add(l, r, Tag(x));
        } else {
            std::cout << zkw.query(l, r).get() << '\n';
        }
    }

    return 0;
}
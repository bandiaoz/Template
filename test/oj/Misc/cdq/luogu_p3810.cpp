#include <bits/stdc++.h>
#include "src/DataStruct/Fenwick/MonoBIT.h"

/*
[P3810 【模板】三维偏序（陌上花开）](https://www.luogu.com.cn/problem/P3810)
[status](https://www.luogu.com.cn/record/204904553)
*/
/**
 * 有 n 个元素，每个元素有三个属性 (a, b, c)，f(i) 表示满足 a_j <= a_i, b_j <= b_i, c_j <= c_i 且 j != i 的 j 的个数。
 * 对于 d = 0...n - 1，求 f(i) = d 的 i 的个数。
 * 
 * 在一些离线的「动态问题」中，cdq 分治提供了一种付出 $O(\log M)$ 的代价，将其转换成若干静态子问题的离线求解框架，其中 $M$ 是操作数量。
 * 有时也称 cdq 分治为 「对时间的分治算法」。
 * cdq 分治的原理基于以下事实：对于每个查询操作，其结果 ans[i] = [1, i - 1] 中所有修改对其造成影响的叠加，这里的「叠加」需要能够比较方便的维护，例如 sum/min/max 等。
 * 
 * 第一维可以看成时间轴
 * 静态三维偏序 -> 动态二维偏序 -> cdq -> 静态二维偏序(sort + fenwick)
 * work(l, r) 中，[l, mid) 的第一维小于等于 [mid, r) 的第一维，并且 [l, mid) 和 [mid, r) 已经关于第二维有序，使用 Fenwick 计算 [l, mid) 对 [mid, r) 的贡献。
 * 复杂度为 O(n \log^2 n)
 */

struct Info {
    std::array<int, 3> v;
    int cnt;
    int res;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, max;
    std::cin >> n >> max;
    std::vector<std::array<int, 3>> a(n);
    for (auto &[x, y, z] : a) {
        std::cin >> x >> y >> z;
        x--, y--, z--;
    }

    std::sort(a.begin(), a.end());

    std::vector<Info> b;
    for (int i = 0, j = 0; i < n; i = j) {
        while (j < n && a[i] == a[j]) {
            j++;
        }
        b.push_back({a[i], j - i, 0});
    }

    OY::MonoSumBIT<int> fen(max);
    auto work = [&](auto &&self, int l, int r) -> void {
        if (l + 1 == r) return;
        int mid = (l + r) / 2;
        self(self, l, mid);
        self(self, mid, r);

        for (int i = l, j = mid; j < r; j++) {
            while (i < mid && b[i].v[1] <= b[j].v[1]) {
                fen.add(b[i].v[2], b[i].cnt);
                i++;
            }
            b[j].res += fen.presum(b[j].v[2] + 1);
        }
        for (int i = l, j = mid; j < r; j++) {
            while (i < mid && b[i].v[1] <= b[j].v[1]) {
                fen.add(b[i].v[2], -b[i].cnt);
                i++;
            }
        }

        std::inplace_merge(b.begin() + l, b.begin() + mid, b.begin() + r, [](const auto &lhs, const auto &rhs) {
            return std::tie(lhs.v[1], lhs.v[2]) < std::tie(rhs.v[1], rhs.v[2]);
        });
    };
    work(work, 0, b.size());

    std::vector<int> ans(n);
    for (const auto &[v, cnt, res] : b) {
        ans[res + cnt - 1] += cnt;
    }
    
    for (int i = 0; i < n; i++) {
        std::cout << ans[i] << '\n';
    }

    return 0;
}
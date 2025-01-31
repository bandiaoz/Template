#include <bits/stdc++.h>
#include "src/DataStruct/Fenwick/BIT.h"
#include "src/DataStruct/Fenwick/MonoBIT.h"

using ll = long long;

/*
[颜色与幸运数字](https://ac.nowcoder.com/acm/problem/285800)
[status](https://ac.nowcoder.com/acm/contest/view-submission?submissionId=74298993)
*/
/**
 * 求区间内恰好出现 k 次的颜色总和
 * 类似处理区间颜色种类，离线询问，树状数组维护
 * 将询问按照右端点排序，对于区间 $[1, r]$，如果颜色 $val$ 出现次数 $cnt >= k$，那么：
 * 差分思想，记第 $cnt - k + 1$ 次出现位置为 $p1$，第 $cnt - k$ 次出现位置为 $p2$，那么：
 * 树状数组 $p1$ 位置加 $val$，$p2$ 位置减 $val$
 * 查询 $[l, r]$ 的和，即为答案
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, k;
    std::cin >> n >> m >> k;
    const int N = 1'000'000;
    std::vector<int> a(n);
    std::vector<std::vector<int>> pos(N + 1);
    std::vector<int> cnt(n);
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
        cnt[i] = pos[a[i]].size();
        pos[a[i]].push_back(i);
    }

    std::vector<std::array<int, 3>> query(m);
    for (int i = 0; i < m; i++) {
        auto &[l, r, id] = query[i];
        std::cin >> l >> r;
        l--;
        id = i;
    }
    std::sort(query.begin(), query.end(), [&](const auto &lhs, const auto &rhs) {
        return lhs[1] < rhs[1];
    });

    auto get = [&](int x, int c) {
        return pos[x][c];
    };

    std::vector<ll> ans(m);
    // OY::VectorBIT<ll> fen(n);
    OY::MonoSumBIT<ll> fen(n);
    auto add = [&](int j, int c) {
        int val = a[j];
        if (cnt[j] >= k - 1) {
            fen.add(get(val, cnt[j] - k + 1), val * c);
        }
        if (cnt[j] >= k) {
            fen.add(get(val, cnt[j] - k), -val * c);
        }
    };

    for (int i = 0, j = 0; i < m; i++) {
        auto &[l, r, id] = query[i];
        while (j < r) {
            if (cnt[j] > 0) {
                int last = get(a[j], cnt[j] - 1);
                add(last, -1);
            }
            add(j, 1);
            j++;
        }
        ans[id] = fen.query(l, r);
    }

    for (auto &i : ans) {
        std::cout << i << '\n';
    }

    return 0;
}
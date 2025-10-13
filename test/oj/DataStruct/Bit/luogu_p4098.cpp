#include <bits/stdc++.h>
#include "src/DataStruct/Bit/PersistentBiTrie.h"
#include "src/DataStruct/RMQ/SparseTable.h"

/*
[P4098 [HEOI2013]ALO](https://www.luogu.com.cn/problem/P4098)
[status](https://www.luogu.com.cn/record/239928235)
*/
/*
给定一个长度为 $$n(1 \leq n \leq 5 \times 10^4)$$ 的数组 $$a(0 \leq a_i \leq 10^9)$$，数组中的数两两不同，定义区间权值为：
假设区间内次大值为 $$k$$，那么区间权值为 $$\max \left(k \operatorname{xor} a_i\right)$$，其中 $$a_i$$ 为区间内的任意一个数。
求一个最大的区间权值。

枚举次大值，找尽可能大的区间使得次大值为该值，使用 SparseTable + 二分来找尽可能大的区间，然后使用 PersistentBiTrie 区间最大异或
分两种情况，一种最大值在左边，一种最大值在右边
*/

using Trie = OY::PerBiTrie::CountTree<uint32_t, uint32_t, 30>;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    std::vector<Trie> trie(n + 1);
    trie[0].insert_one(0);
    for (int i = 0; i < n; i++) {
        trie[i + 1] = trie[i].copy();
        trie[i + 1].insert_one(a[i]);
    }

    OY::STMaxTable<int> max(a.begin(), a.end());
    auto get_left = [&](int i) -> std::pair<int, int> {
        if (i == 0 || max.query(0, i) < a[i]) {
            return {-1, -1};
        }
        int left = max.min_left(i - 1, [&](int max) { return max < a[i]; });
        left--;
        if (left > 0) {
            left = max.min_left(left - 1, [&](int max) { return max < a[i]; });
        }
        int right = i;
        if (right + 1 < n) {
            right = max.max_right(right + 1, [&](int max) { return max < a[i]; });
        }
        return {left, right};
    };
    auto get_right = [&](int i) -> std::pair<int, int> {
        if (i == n - 1 || max.query(i, n) < a[i]) {
            return {-1, -1};
        }
        int right = max.max_right(i + 1, [&](int max) { return max < a[i]; });
        right++;
        if (right < n - 1) {
            right = max.max_right(right + 1, [&](int max) { return max < a[i]; });
        }
        int left = i;
        if (left > 0) {
            left = max.min_left(left - 1, [&](int max) { return max < a[i]; });
        }
        return {left, right};
    };
    uint32_t ans = 0;
    for (int i = 0; i < n; i++) {
        for (auto [l, r] : {get_left(i), get_right(i)}) {
            if (l == -1 || r == -1) continue;
            if (l != i) {
                ans = std::max(ans, (trie[i] - trie[l]).max_bitxor(a[i]));
            }
            if (r != i) {
                r++;
                ans = std::max(ans, (trie[r] - trie[i]).max_bitxor(a[i]));
            }
        }
    }

    std::cout << ans << '\n';

    return 0;
}
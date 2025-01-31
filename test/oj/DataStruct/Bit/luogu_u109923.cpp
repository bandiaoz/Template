#include <bits/stdc++.h>
#include "src/DataStruct/Bit/BiTrie.h"

using ll = long long;

/*
[U109923 [Codechef REBXOR]Nikitosh and xor](https://www.luogu.com.cn/problem/U109923)
[status](https://www.luogu.com.cn/record/196623212)
*/
/**
 * 给定一个数组，求数组中两个不重叠的子数组($[l_1, r_1], [l_2, r_2], l_1 \leq r_1 < l_2 \leq r_2$)
 * 使得两个子数组异或和的和最大，即 $\max (a[l_1] \oplus \cdots \oplus a[r_1]) + (a[l_2] \oplus \cdots \oplus a[r_2])$
 * 
 * 枚举 $r_1$，对于前缀和后缀分别维护一个 BiTrie 树
 * 枚举左半部分长度最多为 i，则右半部分长度至少为 n - i
 * 左半部分答案为 left_max = std::max(left_max, pre.max_bitxor(get(0, i)).second);
 * 右半部分答案为 suf.max_bitxor(get(i, n)).second)
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &x : a) {
        std::cin >> x;
    }

    std::vector<int> sum(n + 1);
    for (int i = 0; i < n; i++) {
        sum[i + 1] = sum[i] ^ a[i];
    }
    auto get = [&](int l, int r) { return sum[r] ^ sum[l]; };

    OY::BiTrie::CountTree<uint32_t, uint32_t, 32> pre, suf;
    pre.insert_one(0);
    suf.insert_one(0);
    for (int i = 0; i < n; i++) {
        suf.insert_one(get(i, n));
    }

    uint32_t ans = 0, left_max = 0;
    for (int i = 1; i < n; i++) {
        pre.insert_one(get(0, i));
        suf.erase_one(get(i, n));
        left_max = std::max(left_max, pre.max_bitxor(get(0, i)).second);
        ans = std::max(ans, left_max + suf.max_bitxor(get(i, n)).second);
    }

    std::cout << ans << '\n';

    return 0;
}
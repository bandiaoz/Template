#include <bits/stdc++.h>
#include "src/DataStruct/Bit/BiTrie.h"

using ll = long long;

/*
[P5283 [十二省联考 2019] 异或粽子](https://www.luogu.com.cn/problem/P5283)
[status](https://www.luogu.com.cn/record/239778913)
*/
/*
给定长度为 $$n(1 \leq n \leq 5 \times 10^5)$$ 的数组，区间异或和为 $$\{b_1, \dots, b_{\frac{n(n + 1)}{2}}\}$$，求前 $$k$$ 大区间异或和

做一遍前缀和，将区间异或和转换为任选两个前缀的异或和
如果固定选择一个前缀，那么可以 $$O(L)$$ 求出第 $$k$$ 大异或和（CountTree::kth_bitxor），用堆来维护即可
*/

using Trie = OY::BiTrie::CountTree<uint32_t, uint32_t, 32>;
using Node = Trie::node;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint32_t n, k;
    std::cin >> n >> k;
    std::vector<uint32_t> a(n);
    for (auto &x : a) {
        std::cin >> x;
    }

    std::vector<uint32_t> pre(n + 1);
    std::partial_sum(a.begin(), a.end(), std::next(pre.begin()), std::bit_xor<uint32_t>());

    Trie trie;
    for (auto x : pre) {
        trie.insert_one(x);
    }

    std::priority_queue<std::array<uint32_t, 3>> que;
    for (uint32_t i = 0; i < pre.size(); i++) {
        auto [node, val] = trie.kth_bitxor(pre[i], n);
        que.push({val, n, i});
    }
    
    ll ans = 0;
    k *= 2;
    while (k--) {
        auto [val, rk, id] = que.top();
        que.pop();
        ans += val;
        if (rk - 1 >= 0) {
            auto [node, val] = trie.kth_bitxor(pre[id], rk - 1);
            que.push({val, rk - 1, id});
        }
    }

    std::cout << ans / 2 << "\n";
    
    return 0;
}
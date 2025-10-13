#include <bits/stdc++.h>
#include "src/DataStruct/Bit/PersistentBiTrie.h"

/*
[P4735 最大异或和](https://www.luogu.com.cn/problem/P4735)
[status](https://www.luogu.com.cn/record/237623290)
*/
/*
给定长度为 $$n$$ 的数组，有 $$m$$ 次操作：
1. 在数组末尾添加一个数
2. 查询起点为 $$l \leq s \leq r$$ 的后缀和 $$x$$ 的最大异或和
$$1 \leq n, m \leq 3 \times 10^5, 0 \leq a_i \leq 10^7$$

$$a_s \operatorname{xor} \cdots \operatorname{xor} a_N$$ 可以看成 $$all \operatorname{xor} pre$$，其中 $$all$$ 是所有数的异或和，$$pre$$ 是前 $$num(l - 1 \leq num \leq r - 1)$$ 个数的异或和
*/

using Trie = OY::PerBiTrie::CountTree<uint32_t, uint32_t, 24>;
using Node = Trie::node;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<Trie> trie(1);
    trie.reserve(n + m + 1);
    trie[0].insert_one(0);
    uint32_t all = 0;
    auto add = [&](int x) {
        all ^= x;
        trie.push_back(trie.back().copy());
        trie.back().insert_one(all);
    };
    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        add(x);
    }

    while (m--) {
        char op;
        std::cin >> op;

        if (op == 'A') {
            int x;
            std::cin >> x;
            add(x);
        } else {
            int l, r, x;
            std::cin >> l >> r >> x;
            l--;

            x ^= all;
            if (l == 0) {
                std::cout << trie[r - 1].max_bitxor(x).second << '\n';
            } else {
                std::cout << (trie[r - 1] - trie[l - 1]).max_bitxor(x) << '\n';
            }
        }
    }

    return 0;
}
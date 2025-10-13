#include <bits/stdc++.h>
#include "src/DataStruct/Bit/BiTrie.h"

/*
[CF817E Choosing The Commander](https://codeforces.com/contest/817/problem/E)
[status](https://codeforces.com/contest/817/submission/342850179)
*/
/*
有 $$q(1 \leq q \leq 10^5)$$ 次操作：
1. 可重集合中添加一个数 $$x$$
2. 可重集合删除一个数 $$x$$
3. 给定 $$p, l$$，询问可重集合中有多少个 $$x \operatorname{xor} p < l$$

BitTrie::kth_bitxor 模板题
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    std::cin >> q;

    OY::BiTrie::CountTree<uint32_t, int, 30> trie;
    while (q--) {
        int op;
        std::cin >> op;

        if (op == 1) {
            int x;
            std::cin >> x;
            trie.insert_one(x);
        } else if (op == 2) {
            int x;
            std::cin >> x;
            trie.erase_one(x);
        } else {
            int p, l;
            std::cin >> p >> l;
            std::cout << trie.rank_bitxor(p, l) << '\n';
        }
    }

    return 0;
}
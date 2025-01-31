#include <bits/stdc++.h>
#include "src/DataStruct/Bit/BiTrie.h"

using ll = long long;

/*
[Set Xor-Min](https://judge.yosupo.jp/problem/set_xor_min)(https://github.com/yosupo06/library-checker-problems/issues/499)
[status](https://judge.yosupo.jp/submission/258678)
*/
/**
 * 本题为 01 字典树模板题，需要实现删除功能
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    std::cin >> q;
    OY::BiTrie::Tree<uint32_t, 30> trie;
    while (q--) {
        int op, x;
        std::cin >> op >> x;

        if (op == 0) {
            trie.insert(x);
        } else if (op == 1) {
            trie.erase(x);
        } else {
            std::cout << trie.min_bitxor(x).second << '\n';
        }
    }

    return 0;
}
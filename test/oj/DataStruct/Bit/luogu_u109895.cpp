#include <bits/stdc++.h>
#include "src/DataStruct/Bit/BiTrie.h"

using ll = long long;

/*
[U109895 [HDU4825]Xor Sum](https://www.luogu.com.cn/problem/U109895)
[status](https://www.luogu.com.cn/record/196311028)
*/
/**
 * 给定一个数组，每次询问一个数，求数组中与该数异或值最大的数
 * 使用 BiTrie 树进行查询即可，注意范围为 $[1, 2^{32} - 1]$，需要使用 uint32_t 进行存储
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    OY::BiTrie::Tree<uint32_t, 32> trie;
    for (int i = 0; i < n; i++) {
        uint32_t x;
        std::cin >> x;
        trie.insert(x);
    }

    while (q--) {
        uint32_t x;
        std::cin >> x;
        std::cout << (trie.max_bitxor(x).second ^ x) << '\n';
    }

    return 0;
}
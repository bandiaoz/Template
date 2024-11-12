#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt64.h"
#include "src/String/SequenceHash.h"

/*
[#103. 子串查找」三个朋友](https://loj.ac/p/103)
[status](https://loj.ac/s/2198563)
*/
/**
 * 本题可以使用字符串哈希解决
 */

using mint = OY::mint4611686018427387847;
using table_type = OY::SEQHASH::SeqHashPresumTable<mint>;
using hash_type = table_type::hash_type;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s, t;
    std::cin >> s >> t;

    const int N = 1'000'000;
    hash_type::s_info.set_random_base_and_prepare(N);

    auto val = hash_type(t);

    table_type table(s);
    int ans = 0;
    for (int i = 0; i + t.size() <= s.size(); i++) {
        if (table.query_value(i, i + t.size()) == val.m_val) {
            ans++;
        }
    }

    std::cout << ans << '\n';

    return 0;
}
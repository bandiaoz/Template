#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"
#include "src/Math/Modular/StaticModInt32.h"
#include "src/String/SequenceHash.h"

/*
[P2870 [USACO07DEC] Best Cow Line G](https://www.luogu.com.cn/problem/P2870)
[status(SA)](https://www.luogu.com.cn/record/192132172)
[status(hash)](https://www.luogu.com.cn/record/192136597)
*/
/**
 * 每次从字符串的头或者尾取一个字符，形成最小字典序的字符串
 * 
 * 本题求正向和反向的后缀子串字典序顺序，可以有多种做法
 * 可以使用后缀数组解决，也可以使用字符串哈希解决
 * 对于字符串 s，如果 s < reverse(s)，那么就从 s 的头部取一个字符，否则从尾部取一个字符
 */

void solve_SA() {
    int n;
    std::cin >> n;
    std::string s;
    for (int i = 0; i < n; i++) {
        char c;
        std::cin >> c;
        s += c;
    }

    // '#' 的 ASCII 为 35，小于其他字符的 ASCII
    s = s + "#" + std::string(s.rbegin(), s.rend());
    
    OY::SA::SuffixArray<true, false, 1000000> sa(s);
    std::string ans;
    for (int l = 0, r = n - 1; l <= r; ) {
        if (sa.query_rank(l) < sa.query_rank(n * 2 - r)) {
            ans += s[l++];
        } else {
            ans += s[r--];
        }
    }

    for (int i = 0; i < n; i += 80) {
        std::cout << ans.substr(i, 80) << '\n';
    }
}

using mint = OY::mint998244353;
using table_type = OY::SEQHASH::SeqHashPresumTable<mint>;
using hash_type = table_type::hash_type;

void solve_hash() {
    int n;
    std::cin >> n;
    hash_type::s_info.set_random_base_and_prepare(n);
    std::string s;
    for (int i = 0; i < n; i++) {
        char c;
        std::cin >> c;
        s += c;
    }

    table_type s1(s), s2(s.rbegin(), s.rend());
    std::string ans;
    for (int l = 0, r = n - 1; l <= r; ) {
        if (table_type::compare(s1, l, r, s2, n - 1 - r, n - 1) < 0) {
            ans += s[l++];
        } else {
            ans += s[r--];
        }
    }

    for (int i = 0; i < n; i += 80) {
        std::cout << ans.substr(i, 80) << '\n';
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // solve_SA();
    solve_hash();

    return 0;
}
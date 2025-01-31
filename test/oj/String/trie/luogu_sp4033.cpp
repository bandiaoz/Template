#include <bits/stdc++.h>
#include "src/String/trie/Trie.h"

using ll = long long;

/*
[PHONELST - Phone List](https://www.luogu.com.cn/problem/SP4033)
[status](https://www.luogu.com.cn/record/196229347)
*/
/**
 * 给定若干数字串，判断是否存在一个串是另一个串的前缀
 */

template <typename Node>
struct NodeWrap {
    int cnt;
};

void solve() {
    int n;
    std::cin >> n;

    OY::StaticTrie<NodeWrap, 10> trie;
    trie.reserve(n * 10);
    auto modify = [&](auto *node) { node->cnt++; };
    std::vector<std::string> numbers(n);
    for (auto &s : numbers) {
        std::cin >> s;
        auto mapping = [&](int j) { return s[j] - '0'; };
        trie.insert(s.size(), mapping, modify);
    }

    for (auto &s : numbers) {
        auto mapping = [&](int j) { return s[j] - '0'; };
        if (trie.find(s.size(), mapping)->cnt > 1) {
            std::cout << "NO\n";
            return;
        }
    }
    std::cout << "YES\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
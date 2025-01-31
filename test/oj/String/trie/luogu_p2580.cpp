#include <bits/stdc++.h>
#include "src/String/trie/Trie.h"

using ll = long long;

/*
[P2580 于是他错误的点名开始了](https://www.luogu.com.cn/problem/P2580)
[status](https://www.luogu.com.cn/record/196226754)
*/
/**
 * 给定一个姓名集合，然后点名：判断是否名字是否正确，是否第一次出现
 */

template <typename Node>
struct NodeWrap {
    int flag; // 0: 不在集合中, 1: 未出现, 2: 已出现
};

void solve_trie() {
    int n;
    std::cin >> n;
    OY::StaticTrie<NodeWrap, 26> trie;
    trie.reserve(50 * n);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        trie.insert_lower(s)->flag = 1;
    }

    int m;
    std::cin >> m;
    while (m--) {
        std::string s;
        std::cin >> s;
        if (auto it = trie.find_lower(s); !it || it->flag == 0) {
            std::cout << "WRONG\n";
        } else if (it->flag == 1) {
            it->flag = 2;
            std::cout << "OK\n";
        } else {
            std::cout << "REPEAT\n";
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_trie();

    return 0;
}
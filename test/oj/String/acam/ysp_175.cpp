#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"

/*
[Aho Corasick](https://judge.yosupo.jp/problem/aho_corasick)(https://github.com/yosupo06/library-checker-problems/issues/175)
[status](https://judge.yosupo.jp/submission/319153)
*/
/*
给定 $$n$$ 个模式串，将每个模式串的所有前缀插入 AC 自动机中
对于每个节点，对应一个字符串 $$P_{i, j}$$，即第 $$i$$ 个模式串的前 $$j$$ 个字符，按照 $$(i, j)$$ 的字典序进行编号，0 对应空串
1. 输出 AC 自动机中有多少个本质不同的串
2. 输出每个点的父节点编号
3. 输出每个点的 fail 指针指向的节点编号

需要使用 _get_or_new 来插入字符串来获取路径上的节点
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    OY::ACAM<26> ac;
    const int N = 1'000'000;
    ac.reserve(N);

    std::vector<int> id(N + 1, -1), parent(N + 1, -1);
    std::vector<int> pos(n);
    id[0] = 0;
    int cur = 1;
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        int back = 0;
        for (auto c : s) {
            int p = ac._get_or_new(back, c - 'a');
            if (id[p] == -1) {
                id[p] = cur++;
                parent[p] = back;
            }
            back = p;
        }
        pos[i] = back;
    }

    ac.prepare();

    std::cout << cur << "\n";
    for (int i = 1; i < cur; i++) {
        int x = id[parent[i]];
        int y = id[ac.query_fail(i)];
        std::cout << x << " " << y << "\n";
    }

    for (int i = 0; i < n; i++) {
        std::cout << id[pos[i]] << " \n"[i == n - 1];
    }

    return 0;
}
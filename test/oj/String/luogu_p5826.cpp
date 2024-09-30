#include <bits/stdc++.h>
#include "src/String/FastSequenceAutomaton.h"
#include "src/String/SequenceAutomaton.h"

/*
[P5826 【模板】子序列自动机](https://www.luogu.com.cn/problem/P5826)
[status](https://www.luogu.com.cn/record/175520546)
*/
/**
 * 本题为序列自动机模板题
 * 值域(字符集)小时，适用 FastSequenceAutomaton
 * 值域(字符集)大时，适用 SequenceAutomaton
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int type, n, q, m;
    std::cin >> type >> n >> q >> m;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    if (type <= 2) {
        OY::FastSequenceAutomaton<100> sa(a.begin(), a.end());
        while (q--) {
            int len;
            std::cin >> len;
            std::vector<int> b(len);
            for (auto &i : b) {
                std::cin >> i;
            }
            std::cout << (sa.contains(b.begin(), b.end()) ? "Yes" : "No") << '\n';
        }
    } else {
        OY::SequenceAutomaton_map sa(a.begin(), a.end());
        while (q--) {
            int len;
            std::cin >> len;
            std::vector<int> b(len);
            for (auto &i : b) {
                std::cin >> i;
            }
            std::cout << (sa.contains(b.begin(), b.end()) ? "Yes" : "No") << '\n';
        }
    }

    return 0;
}
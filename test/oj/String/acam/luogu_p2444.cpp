#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"

/*
[P2444 [POI 2000] 病毒](https://www.luogu.com.cn/problem/P2444)
[status](https://www.luogu.com.cn/record/202407206)
*/
/**
 * 有 n 个 01 串病毒代码，问是否存在无限长的 01 串，不包含病毒代码
 * 
 * 在 trie 树上，如果这个点到根的路径上不存在病毒状态，并且 fail 指针指向的点也不是病毒状态，那么这个点是合法点
 * 如果 trie 图中合法点可以构成一个环，则存在无限长的 01 串，不包含病毒代码
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    OY::ACAM<2> ac;
    const int N = 30000;
    ac.reserve(N);
    std::vector<int> virus(N + 1);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        int p = ac.insert(s.begin(), s.end(), [&](char c) { return c - '0'; });
        virus[p] = 1;
    }

    // 此时已经处理好 u 节点的 fail 指针
    ac.prepare([&](int u, int v) {
        virus[u] |= virus[ac.query_fail(u)];
        virus[v] |= virus[u];
    });

    // vis[u] = 0: not visited
    // vis[u] = 1: on stack
    // vis[u] = 2: visited
    std::vector<int> vis(N + 1);
    auto find_cycle = [&](auto &&self, int u) -> bool {
        vis[u] = 1;
        for (int c = 0; c < 2; c++) {
            int v = ac.next(u, c);
            if (virus[v]) continue;
            if (vis[v] == 1) return true;
            if (vis[v] == 0 && self(self, v)) return true;
        }
        vis[u] = 2;
        return false;
    };

    std::cout << (find_cycle(find_cycle, 0) ? "TAK" : "NIE") << "\n";

    return 0;
}
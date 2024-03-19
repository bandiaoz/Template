/**
 * @brief 树哈希
 * @link test problem: https://uoj.ac/problem/763
*/
#include <bits/stdc++.h>

using u64 = unsigned long long;

const u64 mask = std::chrono::steady_clock::now().time_since_epoch().count();

u64 shift(u64 x) {
    x ^= mask;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    x ^= mask;
    return x;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> g(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    std::set<u64> trees;
    std::vector<u64> hash(n);
    auto treeHash = [&](auto &&self, int u, int f) -> u64 {
        hash[u] = 1;
        for (int v : g[u]) {
            if (v == f) continue;
            self(self, v, u);
            hash[u] += shift(hash[v]);
        }
        trees.insert(hash[u]);
        return hash[u];
    };

    treeHash(treeHash, 0, -1);
    std::cout << trees.size() << "\n";

    return 0;
}
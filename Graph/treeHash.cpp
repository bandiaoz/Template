#include <bits/stdc++.h>

using namespace std;
using ll = long long;
using ull = unsigned long long;

const ull mask = std::chrono::steady_clock::now().time_since_epoch().count();

ull shift(ull x) {
    x ^= mask;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    x ^= mask;
    return x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<vector<int>> g(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    set<ull> trees;
    vector<ull> hash(n);
    function<int(int, int)> getHash = [&](int u, int f) {
        hash[u] = 1;
        for (int v : g[u]) {
            if (v == f) continue;
            getHash(v, u);
            hash[u] += shift(hash[v]);
        }
        trees.insert(hash[u]);
        return hash[u];
    };

    getHash(0, -1);
    cout << trees.size() << "\n";

    return 0;
}
// test problem: https://uoj.ac/problem/763
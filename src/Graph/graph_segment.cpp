#include <bits/stdc++.h>

using ll = long long;

/**
 * @brief 线段树优化建图
 * @link https://codeforces.com/contest/786/problem/B
 * @link https://codeforces.com/contest/1158/problem/C
 * @note 实际有 n 个点，编号为 0 ~ n - 1，虚拟节点编号为 n ~ node - 1
 */
struct graph_segment {
    int node, n, sz;
    std::vector<int> up, down;
    std::vector<std::vector<std::pair<int, int>>> g;
    std::vector<int> w;
    graph_segment(int _n) : n(_n) {
        node = n, sz = 1;
        while (sz < n) sz <<= 1;
        up.assign(2 * sz, -1);
        down.assign(2 * sz, -1);
        g.resize(n);
        build(0, 0, sz);
    }
    void addEdge(int u, int v, int wi) {
        g[u].emplace_back(v, w.size());
        w.push_back(wi);
    }
    // u -> [l, r)
    void addEdges(int u, std::pair<int, int> v, int wi) {
        addEdges(u, v.first, v.second, wi, 2, 0, 0, sz);
    }
    // [l, r) -> v
    void addEdges(std::pair<int, int> u, int v, int wi) {
        addEdges(v, u.first, u.second, wi, 3, 0, 0, sz);
    }
    std::vector<ll> dijkstra(int s) {
        std::vector<ll> dis(node, -1);
        std::priority_queue<std::pair<ll, int>> pq;
        pq.emplace(0, s);
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (dis[u] != -1) {
                continue;
            }
            dis[u] = -d;
            for (auto [v, j] : g[u]) {
                pq.emplace(d - w[j], v);
            }
        }
        return std::vector(dis.begin(), dis.begin() + n);
    }
 
private:
    int new_node() {
        g.emplace_back();
        return node++;
    }
    void build(int x, int lx, int rx) {
        if (lx >= n) return;
        if (rx - lx == 1) {
            up[x] = down[x] = lx;
            return;
        }
        up[x] = new_node();
        down[x] = new_node();
        int mid = (lx + rx) >> 1;
        build(2 * x + 1, lx, mid);
        build(2 * x + 2, mid, rx);
        if (up[2 * x + 1] != -1) addEdge(up[x], up[2 * x + 1], 0);
        if (up[2 * x + 2] != -1) addEdge(up[x], up[2 * x + 2], 0);
        if (down[2 * x + 1] != -1) addEdge(down[2 * x + 1], down[x], 0);
        if (down[2 * x + 2] != -1) addEdge(down[2 * x + 2], down[x], 0);
    }
    void addEdges(int u, int l, int r, int w, int type, int x, int lx, int rx) {
        if (lx >= r || rx <= l) return;
        if (lx >= l && rx <= r) {
            if (type <= 2) addEdge(u, up[x], w);
            else addEdge(down[x], u, w);
            return;
        }
        int mid = (lx + rx) >> 1;
        addEdges(u, l, r, w, type, 2 * x + 1, lx, mid);
        addEdges(u, l, r, w, type, 2 * x + 2, mid, rx);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q, s;
    std::cin >> n >> q >> s;
    s--;
    
    graph_segment g(n);
    for (int i = 0; i < q; i++) {
        int op;
        std::cin >> op;
        if (op == 1) {
            int u, v, w;
            std::cin >> u >> v >> w;
            u--, v--;
            g.addEdge(u, v, w);
        } else if (op == 2) {
            int u, l, r, w;
            std::cin >> u >> l >> r >> w;
            u--, l--;
            g.addEdges(u, {l, r}, w);
        } else {
            int u, l, r, w;
            std::cin >> u >> l >> r >> w;
            u--, l--;
            g.addEdges({l, r}, u, w);
        }
    }

    for (auto x : g.dijkstra(s)) {
        std::cout << x << ' ';
    }

    return 0;
}
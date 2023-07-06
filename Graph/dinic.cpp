#include <bits/stdc++.h>

template<class cap_t>
struct Flow {
    static constexpr cap_t INF = numeric_limits<cap_t>::max();
    int n;
    struct Edge {
        int to;
        cap_t cap;
        Edge(int to, cap_t cap) : to(to), cap(cap) {}
    };
    std::vector<Edge> e;
    std::vector<std::vector<int>> g;
    std::vector<int> cur, h;
    Flow(int n) : n(n), g(n) {}
    bool bfs(int s, int t) {
        h.assign(n, -1);
        queue<int> que;
        h[s] = 0;
        que.push(s);
        while (!que.empty()) {
            int u = que.front();
            que.pop();
            for (int j : g[u]) {
                int v = e[j].to;
                cap_t c = e[j].cap;
                if (c > 0 && h[v] == -1) {
                    h[v] = h[u] + 1;
                    if (v == t) return true;
                    que.push(v);
                }
            }
        }
        return false;
    }
    cap_t dfs(int u, int t, cap_t f) {
        if (u == t) return f;
        cap_t r = f;
        for (int &i = cur[u]; i < int(g[u].size()); ++i) {
            int j = g[u][i];
            int v = e[j].to;
            cap_t c = e[j].cap;
            if (c > 0 && h[v] == h[u] + 1) {
                cap_t a = dfs(v, t, min(r, c));
                e[j].cap -= a;
                e[j ^ 1].cap += a;
                r -= a;
                if (r == 0) return f;
            }
        }
        return f - r;
    }
    void addEdge(int u, int v, cap_t c) {
        g[u].push_back(e.size());
        e.emplace_back(v, c);
        g[v].push_back(e.size());
        e.emplace_back(u, 0);
    }
    cap_t maxFlow(int s, int t) {
        cap_t ans = 0;
        while (bfs(s, t)) {
            cur.assign(n, 0);
            ans += dfs(s, t, INF);
        }
        return ans;
    }
};

using ll = long long;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m, source, sink;
    std::cin >> n >> m >> source >> sink;
    source--, sink--;
    Flow<ll> flow(n);
    for (int i = 0; i < m; ++i) {
        int u, v, c;
        std::cin >> u >> v >> c;
        u--, v--;
        flow.addEdge(u, v, c);
    }

    std::cout << flow.maxFlow(source, sink) << "\n";
    
    return 0;
}

// test problem: https://loj.ac/p/101
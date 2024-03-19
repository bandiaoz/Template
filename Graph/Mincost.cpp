#include <bits/stdc++.h>

template <typename cap_t, typename cost_t>
struct Mincost {
    static constexpr cost_t INF_COST = std::numeric_limits<cost_t>::max();
    static constexpr cap_t INF_CAP = std::numeric_limits<cap_t>::max();
    int n;
    struct Edge {
        int to;
        cap_t cap;
        cost_t cost;
        Edge(int to, cap_t cap, cost_t cost) : to(to), cap(cap), cost(cost) {}
    };
    std::vector<Edge> e;
    std::vector<std::vector<int>> g;
    std::vector<int> cur, pre;
    std::vector<bool> vis;
    std::vector<cost_t> dis;
    Mincost(int n) : n(n), g(n), vis(n) {}
    void addEdge(int u, int v, cap_t c, cost_t w) {
        g[u].push_back(e.size());
        e.emplace_back(v, c, w);
        g[v].push_back(e.size());
        e.emplace_back(u, 0, -w);
    }
    bool spfa(int s, int t) {
        pre.assign(n, -1);
        dis.assign(n, INF_COST);
        std::queue<int> que;
        que.push(s);
        dis[s] = 0;
        while (!que.empty()) {
            int u = que.front();
            que.pop();
            vis[u] = false;
            for (auto j : g[u]) {
                auto [v, c, w] = e[j];
                if (c > 0 && dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    pre[v] = j;
                    if (!vis[v]) {
                        que.push(v);
                        vis[v] = true;
                    }
                }
            }
        }
        return dis[t] != INF_COST;
    }
    std::pair<cap_t, cost_t> dfs(int u, int t, cap_t f) {
        if (u == t) return {f, 0};
        vis[u] = true;
        cap_t r = f;
        cost_t p = 0;
        for (int &i = cur[u]; i < int(g[u].size()); ++ i) {
            int j = g[u][i];
            auto [v, c, w] = e[j];
            if (!vis[v] && c > 0 && dis[v] == dis[u] + w) {
                auto a = dfs(v, t, std::min(c, r));
                e[j].cap -= a.first;
                e[j ^ 1].cap += a.first;
                r -= a.first;
                p += a.first * w + a.second;
                if (r == 0) break;
            }
        }
        vis[u] = false;
        return {f - r, p};
    }
    void augment(int s, int t, std::pair<cap_t, cost_t> &ans) {
        int p = t;
        cap_t _f = INF_CAP;
        while (pre[p] != -1) {
            _f = std::min(_f, e[pre[p]].cap);
            p = e[pre[p] ^ 1].to;
        }
        ans.first += _f;
        ans.second += _f * dis[t];
        p = t;
        while (pre[p] != -1) {
            e[pre[p]].cap -= _f;
            e[pre[p] ^ 1].cap += _f;
            p = e[pre[p] ^ 1].to;
        }
    }
    // select dfs or augment
    // dfs() can multiple augment
    // augment() can augment a minimum cost flow
    std::pair<cap_t, cost_t> maxFlowMinCost(int s, int t) {
        std::pair<cap_t, cost_t> ans = {0, 0};
        while (spfa(s, t)) {
            cur.assign(n, 0);
            auto res = dfs(s, t, INF_CAP);
            ans.first += res.first;
            ans.second += res.second;

            // augment(s, t, ans);
        }
        return ans;
    }
};

using ll = long long;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    Mincost<ll, ll> flow(n);
    const int source = 0, sink = n - 1;

    for (int i = 0; i < m; ++ i) {
        int u, v;
        ll c, w;
        std::cin >> u >> v >> c >> w;
        u--, v--;
        flow.addEdge(u, v, c, w);
    }

    auto ans = flow.maxFlowMinCost(source, sink);
    std::cout << ans.first << " " << ans.second << "\n";

    return 0;
};

// test problem: https://loj.ac/p/102
#include <bits/stdc++.h>

using ll = long long;

constexpr ll inf = 1e12;

/**
 * @brief 二分图带权匹配
 * @tparam T the type of weight
 * @link https://uoj.ac/problem/80
 */
template<class T>
struct KuhnMunkres {
    int n;
    std::vector<std::vector<T>> w;
    std::vector<T> hl, hr;
    std::vector<int> fl, fr, pre;
    KuhnMunkres(int n) : n(n), w(n, std::vector<T>(n)), 
        hl(n), hr(n), fl(n, -1), fr(n, -1), pre(n) {}

    std::vector<int> km() {
        for (int i = 0; i < n; i++) {
            hl[i] = *max_element(w[i].begin(), w[i].end());
        }
        for (int s = 0; s < n; s++) {
            [&](int s) {
                std::vector<T> slack(n, inf);
                std::vector<int> vl(n), vr(n);
                queue<int> q;
                q.push(s);
                vr[s] = 1;
                auto check = [&](int u) {
                    vl[u] = 1;
                    if (fl[u] != -1) {
                        q.push(fl[u]);
                        vr[fl[u]] = 1;
                        return 1;
                    }
                    while (u != -1) swap(u, fr[fl[u] = pre[u]]);
                    return 0;
                };
                while (true) {
                    while (not q.empty()) {
                        int u = q.front();
                        q.pop();
                        for (int i = 0; i < n; i++) {
                            T d = hl[i] + hr[u] - w[i][u];
                            if (not vl[i] and slack[i] >= d) {
                                pre[i] = u;
                                if (d)
                                    slack[i] = d;
                                else if (not check(i))
                                    return;
                            }
                        }
                    }
                    T d = inf;
                    for (int i = 0; i < n; i++)
                        if (not vl[i]) d = min(d, slack[i]);
                    for (int i = 0; i < n; i++) {
                        if (vl[i]) {
                            hl[i] += d;
                        } else {
                            slack[i] -= d;
                        }
                    }
                    for (int i = 0; i < n; i++) {
                        if (vr[i]) hr[i] -= d;
                    }
                    for (int i = 0; i < n; i++) {
                        if (not vl[i] and not slack[i] and not check(i)) return;
                    }
                }
            }(s);
        }
        return fl;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n1, n2, m;
    std::cin >> n1 >> n2 >> m;
    int n = std::max(n1, n2);
    KuhnMunkres<ll> km(n);
    while (m--) {
        int u, v, c;
        std::cin >> u >> v >> c;
        u--, v--;
        km.w[u][v] = c;
    }

    auto res = km.km();
    ll ans = 0;
    for (int i = 0; i < n1; i++) {
        if (res[i] != -1) {
            ans += km.w[i][res[i]];
        }
    }
    std::cout << ans << "\n";
    for (int i = 0; i < n1; i++) {
        std::cout << (km.w[i][res[i]] ? res[i] + 1 : 0) << " \n"[i == n1 - 1];
    }

    return 0;
}
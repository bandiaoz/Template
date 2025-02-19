#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

using ll = long long;

/*
[CF467D Fedor and Essay](https://codeforces.com/contest/467/problem/D)
[status](https://codeforces.com/contest/467/submission/306314990)
*/
/**
 * 给定若干单词组成的文章，通过同义词替换，求出最终文章包含尽可能少的 R，如果存在多种情况，最小化文章长度
 * 
 * 将同义词关系建图，注意是建反图，然后缩点，在 DAG 上 dp
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<std::string> words(n);
    std::map<std::string, int> word_id;
    auto get_id = [&](std::string &s) {
        for (auto &c : s) {
            c = std::tolower(c);
        }
        if (!word_id.contains(s)) {
            word_id[s] = word_id.size();
        }
        return word_id[s];
    };
    for (auto &s : words) {
        std::cin >> s;
        get_id(s);
    }

    int m;
    std::cin >> m;
    std::vector<std::pair<int, int>> edges(m);
    for (int i = 0; i < m; i++) {
        std::string u, v;
        std::cin >> u >> v;
        edges[i] = {get_id(u), get_id(v)};
    }

    std::vector<int> a(word_id.size());
    std::vector<int> len(word_id.size());
    for (auto &[s, id] : word_id) {
        a[id] = std::count(s.begin(), s.end(), 'r');
        len[id] = s.size();
    }


    OY::SCC::Graph graph(word_id.size(), m);
    for (auto [u, v] : edges) {
        graph.add_edge(v, u);
    }

    auto scc = graph.calc();
    auto groups = scc.get_groups();

    const int inf = 1e9;
    std::vector dp(groups.size(), std::pair{inf, inf});
    for (int i = 0; i < groups.size(); i++) {
        for (auto u : groups[i]) {
            dp[i] = std::min(dp[i], std::pair{a[u], len[u]});
        }
        for (auto u : groups[i]) {
            graph(u, [&](int v) {
                if (scc.query(v) != i) {
                    dp[scc.query(v)] = std::min(dp[scc.query(v)], dp[i]);
                }
            });
        }
    }

    std::pair<ll, ll> ans{0, 0};
    for (auto word : words) {
        auto [val, len] = dp[scc.query(word_id[word])];
        ans.first += val;
        ans.second += len;
    }

    std::cout << ans.first << " " << ans.second << "\n";

    return 0;
}
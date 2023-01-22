#include <bits/stdc++.h>

using namespace std;
using ll = long long;

// HK O(sqrt(n)m)
class BipartiteMatching {
private:
    int nl, nr;
    vector<vector<int>> g;
    int time;
    vector<int> matchx, matchy, vis, levelx, levely;
    bool find_match(int x) {
        for (auto y : g[x]) {
            if (levely[y] == levelx[x] + 1 && vis[y] != time) {
                vis[y] = time;
                if (matchy[y] == -1 || find_match(matchy[y])) {
                    matchy[y] = x;
                    matchx[x] = y;
                    return true;
                }
            }
        }
        return false;
    }
    bool find_path() {
        vector<int> q;
        for (int x = 0; x < nl; ++x) {
            if (matchx[x] >= 0) {
                levelx[x] = 0;
            } else {
                levelx[x] = 1;
                q.push_back(x);
            }
        }
        bool found = false;
        fill(levely.begin(), levely.end(), 0);
        for (int i = 0; i < q.size(); ++i) {
            int x = q[i];
            for (auto y : g[x]) {
                if (levely[y] == 0) {
                    levely[y] = levelx[x] + 1;
                    if (int z = matchy[y]; z >= 0) {
                        levelx[z] = levely[y] + 1;
                        q.push_back(z);
                    } else {
                        found = true;
                    }
                }
            }
        }
        return found;
    }

public:
    BipartiteMatching(int nl, int nr) : nl(nl), nr(nr), g(nl), time(0) {}
    void addEdge(int x, int y) {
        assert(0 <= x && x < nl && 0 <= y && y < nr);
        g[x].push_back(y);
    }
    pair<int, vector<int>> solve() {
        matchx.resize(nl, -1);
        matchy.resize(nr, -1);
        vis.resize(nr, 0);
        levelx.resize(nl);
        levely.resize(nr);

        int ans = 0;
        time = 0;
        while (find_path()) {
            time++;
            for (int x = 0; x < nl; ++x) {
                if (matchx[x] == -1 && find_match(x)) ans += 1;
            }
        }
        return pair(ans, matchx);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int nl, nr, m;
    cin >> nl >> nr >> m;
    BipartiteMatching graph(nl, nr);
    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        x--, y--;
        graph.addEdge(x, y);
    }

    auto [res, match] = graph.solve();
    cout << res << "\n";
    for (int i = 0; i < nl; ++i) {
        cout << match[i] + 1 << " \n"[i == nl - 1];
    }

    return 0;
}
// test problem: https://uoj.ac/problem/78
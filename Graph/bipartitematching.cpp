#include <bits/stdc++.h>

/**
 * @brief 二分图匹配
 * @link https://uoj.ac/problem/78
 * @link https://judge.yosupo.jp/problem/bipartitematching
 * @note HK O(sqrt(n)m)
*/
class BipartiteMatching {
private:
    int nl, nr;
    std::vector<std::vector<int>> g;
    int time;
    std::vector<int> matchx, matchy, vis, levelx, levely;
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
        std::vector<int> q;
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
    /**
     * @brief  左部有 nl 个点，右部有 nr 个点，添加一条从 x 到 y 的边
     * @param x 0 <= x < nl
     * @param y 0 <= y < nr
     */
    void addEdge(int x, int y) {
        assert(0 <= x && x < nl && 0 <= y && y < nr);
        g[x].push_back(y);
    }
    /**
     * @brief 求最大匹配
     * @return std::pair<int, std::vector<int>> 匹配数和匹配方案
     *         匹配方案 matchx[i] = j 表示左边第 i 个点匹配右边第 j 个点
     *         如果 matchx[i] = -1 表示左边第 i 个点没有匹配
    */
    std::pair<int, std::vector<int>> solve() {
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
        return std::pair(ans, matchx);
    }
};
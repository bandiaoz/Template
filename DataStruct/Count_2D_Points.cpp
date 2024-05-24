#include <bits/stdc++.h>
#include "DataStruct/Fenwick.hpp"

/**
 * @brief Count the number of points in a rectangle
 * @link https://www.luogu.com.cn/problem/P2163
 * @note 把所有的点按照 x 坐标排序，询问的时候双指针维护一个区间，用 Fenwick 维护 [y1, y2] 范围内有多少点 y 坐标
 * @param T the type of x, y
*/
template <typename T>
struct Count_2D_Points {
    std::vector<std::pair<T, T>> a;
    std::vector<T> bx, by;
    std::vector<std::array<T, 4>> q;
    std::vector<int> ans;

    Count_2D_Points() {}
    void addPoint(T x, T y) {
        a.emplace_back(x, y);
        bx.push_back(x);
        by.push_back(y);
    }
    /**
     * 询问矩形 (x1, y1) 到 (x2, y2) 内的点的个数，包含边界
    */
    void addQuery(T x1, T y1, T x2, T y2, int id) {
        bx.push_back(x1 - 1);
        bx.push_back(x2);
        by.push_back(y1 - 1);
        by.push_back(y2);
        q.push_back({x2, y2, id, 1});
        q.push_back({x2, y1 - 1, id, -1});
        q.push_back({x1 - 1, y2, id, -1});
        q.push_back({x1 - 1, y1 - 1, id, 1});
    }
    /**
     * @return 对每次询问，返回矩形内有多少个点
    */
    std::vector<int> work() {
        sort(bx.begin(), bx.end());
        sort(by.begin(), by.end());
        bx.erase(unique(bx.begin(), bx.end()), bx.end());
        by.erase(unique(by.begin(), by.end()), by.end());
        for (auto &[x, y] : a) {
            x = lower_bound(bx.begin(), bx.end(), x) - bx.begin();
            y = lower_bound(by.begin(), by.end(), y) - by.begin();
        }
        for (auto &[x, y, id, _] : q) {
            x = lower_bound(bx.begin(), bx.end(), x) - bx.begin();
            y = lower_bound(by.begin(), by.end(), y) - by.begin();
        }

        sort(a.begin(), a.end());
        sort(q.begin(), q.end());
        Fenwick<int> fen(by.size());
        ans.resize(q.size());
        for (int i = 0, j = 0; i < q.size(); i++) {
            while (j < a.size() && a[j].first <= q[i][0]) {
                fen.add(a[j].second, 1);
                j++;
            }
            ans[q[i][2]] += q[i][3] * fen.sum(q[i][1] + 1);
        }
        return ans;
    }
};
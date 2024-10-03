#include <bits/stdc++.h>
#include "src/DataStruct/Fenwick.hpp"

/**
 * @brief 离线二维【单点加，矩形区域和查询】表
 * @link https://www.luogu.com.cn/problem/P2163
 * @link https://judge.yosupo.jp/problem/rectangle_sum
 * @note 把所有的点按照 x 坐标排序，询问的时候双指针维护一个区间，用 Fenwick 维护 [y1, y2] 范围内的权值和
 * @param point_t the type of x, y
 * @param weight_t the type of weight
*/
template <typename point_t, typename weight_t=int>
struct Count_2D_Points {
    std::vector<std::tuple<point_t, point_t, weight_t>> a;
    std::vector<point_t> bx, by;
    std::vector<std::tuple<point_t, point_t, size_t, int>> q;
    std::vector<weight_t> ans;

    Count_2D_Points() {}
    /**
     * 添加一个点 (x, y) 权值为 w，如果没有权值，求点的数量，则 w 默认为 1
     */
    void addPoint(point_t x, point_t y, weight_t w = 1) {
        a.emplace_back(x, y, w);
        bx.push_back(x);
        by.push_back(y);
    }
    /**
     * 询问 (x, y) 权值和，for x1 <= x < x2, y1 <= y < y2
     * 将一次询问转换为四次询问的总和，每次询问二维前缀
    */
    void addQuery(point_t x1, point_t y1, point_t x2, point_t y2, size_t id) {
        x1--, y1--, x2--, y2--;
        bx.push_back(x1);
        bx.push_back(x2);
        by.push_back(y1);
        by.push_back(y2);
        q.emplace_back(x2, y2, id, 1);
        q.emplace_back(x2, y1, id, -1);
        q.emplace_back(x1, y2, id, -1);
        q.emplace_back(x1, y1, id, 1);
    }
    /**
     * @return 对每次询问，返回矩形内有多少个点
    */
    std::vector<weight_t> work() {
        sort(bx.begin(), bx.end());
        sort(by.begin(), by.end());
        bx.erase(unique(bx.begin(), bx.end()), bx.end());
        by.erase(unique(by.begin(), by.end()), by.end());
        for (auto &[x, y, w] : a) {
            x = lower_bound(bx.begin(), bx.end(), x) - bx.begin();
            y = lower_bound(by.begin(), by.end(), y) - by.begin();
        }
        for (auto &[qx, qy, id, pn] : q) {
            qx = lower_bound(bx.begin(), bx.end(), qx) - bx.begin();
            qy = lower_bound(by.begin(), by.end(), qy) - by.begin();
        }

        sort(a.begin(), a.end());
        sort(q.begin(), q.end());
        Fenwick<weight_t> fen(by.size());
        ans.resize(q.size());
        for (int i = 0, j = 0; i < q.size(); i++) {
            auto [qx, qy, id, pn] = q[i];
            while (j < a.size() && std::get<0>(a[j]) <= qx) {
                auto [x, y, w] = a[j];
                fen.add(y, w);
                j++;
            }
            ans[id] += pn * fen.sum(qy + 1);
        }
        return ans;
    }
};
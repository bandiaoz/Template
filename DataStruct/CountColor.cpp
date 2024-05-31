#include <bits/stdc++.h>
#include "Fenwick.hpp"

/**
 * @brief 区间数颜色
 * @note 对询问按照右端点排序，用 Fenwick 维护每个颜色最后出现的位置
 * @link https://www.luogu.com.cn/problem/P1972
 */
struct CountColor {
    std::vector<int> color;
    std::vector<std::array<int, 3>> query;
    std::vector<int> ans;

    CountColor() {}
    CountColor(const std::vector<int> &color) : color(color) {}
    void addQuery(int l, int r, int id) {
        query.push_back({l, r, id});
    }
    std::vector<int> work() {
        auto b = color;
        std::sort(b.begin(), b.end());
        b.erase(std::unique(b.begin(), b.end()), b.end());
        for (auto &i : color) {
            i = std::lower_bound(b.begin(), b.end(), i) - b.begin();
        }

        std::sort(query.begin(), query.end(), [&](const auto &lhs, const auto &rhs) {
            return lhs[1] < rhs[1];
        });

        std::vector<int> pos(b.size(), -1);
        int right = 0;
        Fenwick<int> fen(color.size());
        ans.resize(query.size());
        for (auto [l, r, id] : query) {
            while (right < r) {
                if (pos[color[right]] != -1) {
                    fen.add(pos[color[right]], -1);
                }
                fen.add(right, 1);
                pos[color[right]] = right;
                right++;
            }
            ans[id] = fen.rangeSum(l, r);
        }
        return ans;
    }
};
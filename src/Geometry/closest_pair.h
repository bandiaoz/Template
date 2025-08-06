#ifndef CLOSEST_PAIR_H
#define CLOSEST_PAIR_H

#include <vector>
#include <tuple>
#include <algorithm>
#include <limits>
#include <set>

/**
 * @brief 最近点对
 * @param pts 点集
 * @return 最近点对距离，两个点
 */
template <class T, class SqrType = T>
std::tuple<SqrType, std::pair<T, T>, std::pair<T, T>> closest_pair(std::vector<std::pair<T, T>> pts) {
    int n = pts.size();
    std::sort(pts.begin(), pts.end());

    auto sq = [&](T a) -> SqrType { return SqrType(a) * a; };
    auto dist2 = [&](std::pair<T, T> a, std::pair<T, T> b) -> SqrType {
        return sq(a.first - b.first) + sq(a.second - b.second);
    };

    SqrType d = std::numeric_limits<SqrType>::max();
    std::pair<T, T> pa, pb;
    auto update = [&](std::pair<T, T> a, std::pair<T, T> b) {
        if (auto nd = dist2(a, b); nd < d) {
            d = nd, pa = a, pb = b;
        }
    };

    auto st = std::multiset<std::pair<T, T>, decltype([](std::pair<T, T> a, std::pair<T, T> b) { return a.second < b.second; })>();
    auto its = std::vector<typename decltype(st)::const_iterator>(n);

    for (int i = 0, f = 0; i < n; i++) {
        auto p = pts[i];
        while (f < i && sq(p.first - pts[f].first) >= d) {
            st.erase(its[f++]);
        }
        auto u = st.upper_bound(p);
        {
            auto t = u;
            while (true) {
                if (t == st.begin()) break;
                t = std::prev(t);
                update(*t, p);
                if (sq(p.second - t->second) >= d) break;
            }
        }
        {
            auto t = u;
            while (true) {
                if (t == st.end()) break;
                if (sq(p.second - t->second) >= d) break;
                update(*t, p);
                t = std::next(t);
            }
        }
        its[i] = st.emplace_hint(u, p);
    }

    return {d, pa, pb};
}

#endif
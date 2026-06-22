#ifndef __OY_GEO_CONVEX__
#define __OY_GEO_CONVEX__

#include "Polygon.h"

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <tuple>
#include <vector>

namespace OY {
    namespace GEO {
        /**
         * @brief 凸多边形。构造时会对给定点集求凸包，结果为逆时针顺序。
         */
        template <typename Tp>
        struct Convex : Polygon<Tp> {
            using base_type = Polygon<Tp>;
            using point_type = Point<Tp>;
            using base_type::p;

            Convex() = default;
            explicit Convex(int n) : base_type(n) {}
            explicit Convex(const std::vector<point_type> &points) : base_type(convex_hull(points)) {}
            Convex(std::initializer_list<point_type> points)
                : Convex(std::vector<point_type>(points)) {}

            /**
             * @brief Andrew 算法求凸包；keep_collinear 为 true 时保留边界共线点。
             */
            static std::vector<point_type> convex_hull(std::vector<point_type> points,
                                                       bool keep_collinear = false) {
                std::sort(points.begin(), points.end());
                points.erase(std::unique(points.begin(), points.end()), points.end());
                if (points.size() <= 1) return points;
                if (keep_collinear && _all_collinear(points)) return points;

                auto bad = [&](const point_type &a, const point_type &b, const point_type &c) {
                    int turn = sgn(cross(b - a, c - b));
                    return keep_collinear ? turn < 0 : turn <= 0;
                };
                std::vector<point_type> lower, upper;
                for (auto &q : points) {
                    while (lower.size() >= 2 && bad(lower[lower.size() - 2], lower.back(), q)) {
                        lower.pop_back();
                    }
                    lower.push_back(q);
                }
                for (int i = points.size() - 1; i >= 0; i--) {
                    auto &q = points[i];
                    while (upper.size() >= 2 && bad(upper[upper.size() - 2], upper.back(), q)) {
                        upper.pop_back();
                    }
                    upper.push_back(q);
                }
                lower.pop_back();
                upper.pop_back();
                lower.insert(lower.end(), upper.begin(), upper.end());
                return lower;
            }

            /**
             * @brief 点 q 与凸包的位置关系：0 外部，1 内部，2 边界。
             * @note O(log n)。要求顶点为逆时针凸包顺序且没有冗余共线点；构造函数会满足这一点。
             */
            template <typename Up>
            int relation(const Point<Up> &q) const {
                int n = this->size();
                if (n == 0) return 0;
                if (n == 1) return sgn(distance(p[0], q)) == 0 ? 2 : 0;
                if (n == 2) return Line<Tp>(p[0], p[1]).point_on_segment(q) ? 2 : 0;

                int left = sgn(cross(p[1] - p[0], q - p[0]));
                int right = sgn(cross(p[n - 1] - p[0], q - p[0]));
                if (left < 0 || right > 0) return 0;
                if (left == 0) return Line<Tp>(p[0], p[1]).point_on_segment(q) ? 2 : 0;
                if (right == 0) return Line<Tp>(p[0], p[n - 1]).point_on_segment(q) ? 2 : 0;

                int l = 1, r = n - 1;
                while (l + 1 < r) {
                    int m = (l + r) / 2;
                    if (sgn(cross(p[m] - p[0], q - p[0])) >= 0) {
                        l = m;
                    } else {
                        r = m;
                    }
                }
                int edge = sgn(cross(p[r] - p[l], q - p[l]));
                if (edge < 0) return 0;
                return edge == 0 ? 2 : 1;
            }

            /**
             * @brief 点 q 是否在凸包内；boundary 为 true 时包含边界。
             */
            template <typename Up>
            bool contains(const Point<Up> &q, bool boundary = true) const {
                int r = relation(q);
                return r == 1 || (boundary && r == 2);
            }

            /**
             * @brief 无限直线 l 是否与凸包有公共点。
             * @note 若 l 退化为点，则判断该点是否在凸包内。
             */
            template <typename Up>
            bool line_intersects(const Line<Up> &l) const {
                if (sgn(l.length()) == 0) return contains(l.s);
                bool pos = false, neg = false;
                for (auto &q : p) {
                    int r = l.relation(q);
                    if (r == 0) return true;
                    pos |= r > 0, neg |= r < 0;
                    if (pos && neg) return true;
                }
                return false;
            }

            /**
             * @brief 凸包是否能被点集唯一确定：每条边除端点外至少有一个给定点。
             */
            template <typename Up>
            bool is_unique(const std::vector<Point<Up>> &points) const {
                if (this->size() < 3 || sgn(this->signed_area2()) == 0) return false;
                for (int i = 0; i < this->size(); i++) {
                    Line<Tp> e(p[i], p[this->next(i)]);
                    bool ok = false;
                    for (auto &q : points) {
                        if (e.point_on_segment_strict(q)) {
                            ok = true;
                            break;
                        }
                    }
                    if (!ok) return false;
                }
                return true;
            }

            /**
             * @brief 旋转卡壳求凸包直径，返回 {最远距离, 点下标一, 点下标二}。
             */
            std::tuple<real_type, int, int> diameter() const {
                int n = this->size();
                if (n == 0) return {0, -1, -1};
                if (n == 1) return {0, 0, 0};
                if (n == 2) return {distance(p[0], p[1]), 0, 1};
                if (sgn(this->signed_area2()) == 0) {
                    int l = 0, r = 0;
                    for (int i = 1; i < n; i++) {
                        if (p[i] < p[l]) l = i;
                        if (p[r] < p[i]) r = i;
                    }
                    return {distance(p[l], p[r]), l, r};
                }

                real_type best = 0;
                int a = 0, b = 0, j = 1;
                auto update = [&](int x, int y) {
                    real_type d = (real_type)(p[x] - p[y]).norm2();
                    if (d > best) best = d, a = x, b = y;
                };
                for (int i = 0; i < n; i++) {
                    int ni = this->next(i);
                    while (true) {
                        int nj = this->next(j);
                        auto cur = cross(p[ni] - p[i], p[j] - p[i]);
                        auto nxt = cross(p[ni] - p[i], p[nj] - p[i]);
                        if (sgn(nxt - cur) <= 0) break;
                        j = nj;
                    }
                    update(i, j);
                    update(ni, j);
                }
                return {std::sqrt(best), a, b};
            }

        private:
            static bool _all_collinear(const std::vector<point_type> &points) {
                for (int i = 2; i < (int)points.size(); i++) {
                    if (sgn(cross(points[1] - points[0], points[i] - points[0])) != 0) {
                        return false;
                    }
                }
                return true;
            }
        };
    }

    template <typename Tp>
    using Convex = GEO::Convex<Tp>;
}

#endif

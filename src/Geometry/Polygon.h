#ifndef __OY_GEO_POLYGON__
#define __OY_GEO_POLYGON__

#include "Line.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <initializer_list>
#include <numeric>
#include <vector>

namespace OY {
    namespace GEO {
        /**
         * @brief 简单多边形，顶点按顺时针或逆时针顺序给出。
         * @note 点与多边形关系返回值：0 外部，1 内部，2 边界。
         */
        template <typename Tp>
        struct Polygon {
            using value_type = Tp;
            using point_type = Point<Tp>;
            std::vector<point_type> p;

            Polygon() = default;
            explicit Polygon(int n) : p(n) {}
            explicit Polygon(const std::vector<point_type> &p) : p(p) {}
            Polygon(std::initializer_list<point_type> p) : p(p) {}

            int size() const { return p.size(); }
            bool empty() const { return p.empty(); }
            int next(int i) const { return i + 1 == size() ? 0 : i + 1; }
            int prev(int i) const { return i == 0 ? size() - 1 : i - 1; }

            point_type &operator[](int i) { return p[i]; }
            const point_type &operator[](int i) const { return p[i]; }

            /**
             * @brief 追加一个顶点；调用者需保证追加后仍按边界顺序排列。
             */
            void add_point(const point_type &q) { p.push_back(q); }

            /**
             * @brief 有向面积的两倍；逆时针为正，顺时针为负。
             */
            calc_type<Tp, Tp> signed_area2() const {
                using res_type = calc_type<Tp, Tp>;
                res_type res = 0;
                for (int i = 0; i < size(); i++) {
                    res += cross(p[i], p[next(i)]);
                }
                return res;
            }

            /**
             * @brief 多边形面积，统一返回 real_type。
             */
            real_type area() const { return std::abs((real_type)signed_area2()) / 2; }

            /**
             * @brief 多边形周长。
             */
            real_type perimeter() const {
                real_type res = 0;
                for (int i = 0; i < size(); i++) {
                    res += distance(p[i], p[next(i)]);
                }
                return res;
            }

            /**
             * @brief 多边形重心；面积为 0 时返回 (0, 0)。
             */
            Point<real_type> centroid() const {
                real_type area2 = signed_area2();
                if (sgn(area2) == 0) return Point<real_type>();
                Point<real_type> res;
                for (int i = 0; i < size(); i++) {
                    int j = next(i);
                    real_type c = (real_type)cross(p[i], p[j]);
                    res += (p[i] + p[j]).template cast<real_type>() * c;
                }
                return res / (area2 * 3);
            }

            /**
             * @brief 点 q 与简单多边形的位置关系：0 外部，1 内部，2 边界。
             */
            template <typename Up>
            int relation(const Point<Up> &q) const {
                int winding = 0;
                for (int i = 0; i < size(); i++) {
                    int j = next(i);
                    Line<Tp> e(p[i], p[j]);
                    if (e.point_on_segment(q)) return 2;
                    int k = sgn(cross(p[j] - p[i], q - p[i]));
                    using res_type = calc_type<Tp, Up>;
                    int d1 = sgn(res_type(p[i].y) - q.y);
                    int d2 = sgn(res_type(p[j].y) - q.y);
                    if (k > 0 && d1 <= 0 && d2 > 0) winding++;
                    if (k < 0 && d2 <= 0 && d1 > 0) winding--;
                }
                return winding == 0 ? 0 : 1;
            }

            /**
             * @brief 点 q 是否在多边形内；boundary 为 true 时包含边界。
             */
            template <typename Up>
            bool contains(const Point<Up> &q, bool boundary = true) const {
                int r = relation(q);
                return r == 1 || (boundary && r == 2);
            }

            /**
             * @brief 线段 seg 是否完全在简单多边形内；boundary 为 true 时允许贴边。
             * @note 做法是按线段参数切开所有边界交点，每一小段取中点判断。
             */
            template <typename Up>
            bool contains_segment(const Line<Up> &seg, bool boundary = true) const {
                if (sgn(seg.norm2()) == 0) return contains(seg.s, boundary);
                if (!contains(seg.s, boundary) || !contains(seg.t, boundary)) return false;

                std::vector<real_type> ts{0, 1};
                auto dir = seg.vec();
                auto add = [&](const Point<real_type> &q) {
                    real_type t;
                    if (std::abs((real_type)dir.x) >= std::abs((real_type)dir.y)) {
                        t = (q.x - (real_type)seg.s.x) / (real_type)dir.x;
                    } else {
                        t = (q.y - (real_type)seg.s.y) / (real_type)dir.y;
                    }
                    if (sgn(t) >= 0 && sgn(t - 1) <= 0) {
                        ts.push_back(std::clamp(t, real_type(0), real_type(1)));
                    }
                };
                for (int i = 0; i < size(); i++) {
                    Line<Tp> e(p[i], p[next(i)]);
                    SegmentIntersection res = e.segment_intersection(seg);
                    if (!res) continue;
                    add(res.p1);
                    if (res.type == 2) add(res.p2);
                }
                std::sort(ts.begin(), ts.end());
                ts.erase(std::unique(ts.begin(), ts.end(), [](real_type x, real_type y) {
                    return sgn(x - y) == 0;
                }), ts.end());

                Point<real_type> s = seg.s.template cast<real_type>();
                Point<real_type> v = dir.template cast<real_type>();
                for (int i = 0; i + 1 < (int)ts.size(); i++) {
                    if (sgn(ts[i + 1] - ts[i]) == 0) continue;
                    Point<real_type> mid = s + v * ((ts[i] + ts[i + 1]) / 2);
                    int r = relation(mid);
                    if (r == 0 || (!boundary && r == 2)) return false;
                }
                return true;
            }

            /**
             * @brief 判断顶点序列是否构成凸多边形；allow_collinear 控制是否允许三点共线。
             */
            bool is_convex(bool allow_collinear = true) const {
                if (size() < 3) return false;
                int dir = 0;
                for (int i = 0; i < size(); i++) {
                    int j = next(i), k = next(j);
                    int turn = sgn(cross(p[j] - p[i], p[k] - p[j]));
                    if (turn == 0) {
                        if (!allow_collinear) return false;
                        continue;
                    }
                    if (dir && dir != turn) return false;
                    dir = turn;
                }
                return true;
            }

            /**
             * @brief 多边形边界上的格点数；要求坐标为整数。
             */
            long long boundary_lattice_points() const {
                long long res = 0;
                for (int i = 0; i < size(); i++) {
                    int j = next(i);
                    long long dx = std::abs((long long)p[j].x - (long long)p[i].x);
                    long long dy = std::abs((long long)p[j].y - (long long)p[i].y);
                    res += std::gcd(dx, dy);
                }
                return res;
            }

            /**
             * @brief 多边形内部格点数，Pick 定理；要求坐标为整数。
             * @note 仅适用于非退化简单格点多边形。
             */
            long long interior_lattice_points() const {
                long long area2 = (long long)signed_area2();
                if (area2 < 0) area2 = -area2;
                return (area2 - boundary_lattice_points() + 2) / 2;
            }
        };
    }

    template <typename Tp>
    using Polygon = GEO::Polygon<Tp>;
}

#endif

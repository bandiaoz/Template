#ifndef __OY_GEO_CIRCLE__
#define __OY_GEO_CIRCLE__

#include "Polygon.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>
#include <vector>

namespace OY {
    namespace GEO {
        /**
         * @brief 点和圆的位置关系。
         */
        enum class CirclePointRelation {
            Outside,     // 点在圆外
            Inside,      // 点在圆内，不含圆周
            Boundary,    // 点在圆周上
        };

        /**
         * @brief 两个圆的位置关系。
         */
        enum class CircleRelation {
            Separate,             // 外离
            ExternallyTangent,    // 外切
            Intersect,            // 相交两点
            InternallyTangent,    // 内切
            Contain,              // 内含
            Same,                 // 重合
        };

        /**
         * @brief 直线和圆的位置关系。
         */
        enum class LineCircleRelation {
            Separate,    // 相离
            Tangent,     // 相切
            Intersect,   // 相交两点
        };

        /**
         * @brief 二维圆。圆心坐标类型为 Tp，半径统一用 real_type。
         */
        template <typename Tp>
        struct Circle {
            using value_type = Tp;
            using point_type = Point<Tp>;
            point_type o;
            real_type r;

            constexpr Circle() : o(), r() {}
            constexpr Circle(const point_type &o, real_type r) : o(o), r(r) {}

            template <typename Up>
            constexpr Circle<Up> cast() const { return Circle<Up>(o.template cast<Up>(), r); }

            /**
             * @brief 两个圆是否重合；浮点坐标和半径按 EPS 判断。
             */
            template <typename Up>
            bool same(const Circle<Up> &rhs) const {
                return sgn(distance(o, rhs.o)) == 0 && sgn(r - rhs.r) == 0;
            }

            template <typename Up>
            bool operator==(const Circle<Up> &rhs) const { return same(rhs); }
            template <typename Up>
            bool operator!=(const Circle<Up> &rhs) const { return !same(rhs); }

            real_type area() const { return pi() * r * r; }
            real_type perimeter() const { return 2 * pi() * r; }

            /**
             * @brief 点 p 与圆的位置关系。
             */
            template <typename Up>
            CirclePointRelation relation(const Point<Up> &p) const {
                int v = sgn(distance(o, p) - r);
                return v > 0 ? CirclePointRelation::Outside :
                       v < 0 ? CirclePointRelation::Inside :
                               CirclePointRelation::Boundary;
            }

            /**
             * @brief 点 p 是否在圆内；boundary 为 true 时包含圆周。
             */
            template <typename Up>
            bool contains(const Point<Up> &p, bool boundary = true) const {
                CirclePointRelation v = relation(p);
                return v == CirclePointRelation::Inside ||
                       (boundary && v == CirclePointRelation::Boundary);
            }

            /**
             * @brief rhs 与当前圆的位置关系。
             */
            template <typename Up>
            CircleRelation relation(const Circle<Up> &rhs) const {
                real_type d = distance(o, rhs.o);
                if (sgn(d) == 0 && sgn(r - rhs.r) == 0) return CircleRelation::Same;
                int outer = sgn(d - r - rhs.r);
                if (outer > 0) return CircleRelation::Separate;
                if (outer == 0) return CircleRelation::ExternallyTangent;
                int inner = sgn(d + std::min(r, rhs.r) - std::max(r, rhs.r));
                if (inner < 0) return CircleRelation::Contain;
                if (inner == 0) return CircleRelation::InternallyTangent;
                return CircleRelation::Intersect;
            }

            /**
             * @brief 当前圆是否完整包含 rhs；boundary 为 true 时允许内切。
             */
            template <typename Up>
            bool contains(const Circle<Up> &rhs, bool boundary = true) const {
                int v = sgn(distance(o, rhs.o) + rhs.r - r);
                return boundary ? v <= 0 : v < 0;
            }

            /**
             * @brief 无限直线 l 与圆的位置关系。
             * @note 若 l 退化成点，仅在该点位于圆周时返回 Tangent。
             */
            template <typename Up>
            LineCircleRelation relation_to_line(const Line<Up> &l) const {
                if (sgn(l.norm2()) == 0) {
                    return relation(l.s) == CirclePointRelation::Boundary ?
                           LineCircleRelation::Tangent :
                           LineCircleRelation::Separate;
                }
                int v = sgn(l.distance_to_line(o) - r);
                return v > 0 ? LineCircleRelation::Separate :
                       v == 0 ? LineCircleRelation::Tangent :
                                LineCircleRelation::Intersect;
            }

            /**
             * @brief 无限直线 l 与圆的交点；l 退化时按单点处理。
             */
            template <typename Up>
            std::vector<Point<real_type>> line_intersections(const Line<Up> &l) const {
                if (sgn(l.norm2()) == 0) {
                    if (relation(l.s) == CirclePointRelation::Boundary) {
                        return {l.s.template cast<real_type>()};
                    }
                    return {};
                }
                Point<real_type> q = l.projection(o);
                real_type d = distance(q, o);
                int v = sgn(d - r);
                if (v > 0) return {};
                if (v == 0) return {q};
                Point<real_type> u = l.vec().template cast<real_type>().unit();
                real_type h = std::sqrt(std::max(real_type(0), r * r - d * d));
                return {q - u * h, q + u * h};
            }

            /**
             * @brief 线段 [s, t] 与圆周的交点。
             */
            template <typename Up>
            std::vector<Point<real_type>> segment_intersections(const Line<Up> &seg) const {
                std::vector<Point<real_type>> res;
                if (sgn(seg.norm2()) == 0) {
                    if (relation(seg.s) == CirclePointRelation::Boundary) {
                        res.push_back(seg.s.template cast<real_type>());
                    }
                    return res;
                }
                for (auto &p : line_intersections(seg)) {
                    if (seg.point_on_segment(p)) _push_unique(res, p);
                }
                return res;
            }

            /**
             * @brief 两圆交点；重合时交点无穷多，返回空 vector。
             */
            template <typename Up>
            std::vector<Point<real_type>> circle_intersections(const Circle<Up> &rhs) const {
                CircleRelation rel = relation(rhs);
                if (rel == CircleRelation::Separate || rel == CircleRelation::Contain ||
                    rel == CircleRelation::Same) return {};
                real_type d = distance(o, rhs.o);
                Point<real_type> u = (rhs.o - o).template cast<real_type>() / d;
                real_type a = (r * r - rhs.r * rhs.r + d * d) / (2 * d);
                Point<real_type> q = o.template cast<real_type>() + u * a;
                real_type h2 = std::max(real_type(0), r * r - a * a);
                if (sgn(h2) == 0) return {q};
                Point<real_type> v = rot90(u) * std::sqrt(h2);
                return {q - v, q + v};
            }

            /**
             * @brief 圆周上距离 p 最近的点；p 为圆心时返回圆心右侧点。
             */
            template <typename Up>
            Point<real_type> nearest_point(const Point<Up> &p) const {
                Point<real_type> c = o.template cast<real_type>();
                if (sgn(r) == 0) return c;
                Point<real_type> v = (p - o).template cast<real_type>();
                if (sgn(v.norm2()) == 0) return c + Point<real_type>(r, 0);
                return c + v * (r / v.norm());
            }

            /**
             * @brief 过点 p 作圆的切线，返回切点；点在圆内时返回空 vector。
             */
            template <typename Up>
            std::vector<Point<real_type>> tangent_points(const Point<Up> &p) const {
                std::vector<Point<real_type>> res;
                CirclePointRelation rel = relation(p);
                if (rel == CirclePointRelation::Inside) return res;
                Point<real_type> c = o.template cast<real_type>();
                Point<real_type> v = (p - o).template cast<real_type>();
                if (rel == CirclePointRelation::Boundary || sgn(r) == 0) {
                    res.push_back(rel == CirclePointRelation::Boundary ?
                                  p.template cast<real_type>() : c);
                    return res;
                }
                real_type d2 = v.norm2();
                Point<real_type> base = c + v * (r * r / d2);
                real_type h = r * std::sqrt(std::max(real_type(0), d2 - r * r)) / d2;
                Point<real_type> off = rot90(v) * h;
                _push_unique(res, base - off);
                _push_unique(res, base + off);
                return res;
            }

            /**
             * @brief 两圆公共面积。
             */
            template <typename Up>
            real_type intersection_area(const Circle<Up> &rhs) const {
                real_type d = distance(o, rhs.o);
                if (sgn(d) == 0 && sgn(r - rhs.r) == 0) return area();
                if (sgn(r) == 0 || sgn(rhs.r) == 0) return 0;
                if (sgn(d - r - rhs.r) >= 0) return 0;
                if (sgn(d + std::min(r, rhs.r) - std::max(r, rhs.r)) <= 0) {
                    real_type x = std::min(r, rhs.r);
                    return pi() * x * x;
                }
                real_type a = 2 * std::acos(_clamp_cos((r * r + d * d - rhs.r * rhs.r) / (2 * r * d)));
                real_type b = 2 * std::acos(_clamp_cos((rhs.r * rhs.r + d * d - r * r) / (2 * rhs.r * d)));
                return r * r * (a - std::sin(a)) / 2 + rhs.r * rhs.r * (b - std::sin(b)) / 2;
            }

            /**
             * @brief 当前圆与简单多边形的公共面积；多边形顶点需按边界顺序给出。
             */
            template <typename Up>
            real_type intersection_area(const Polygon<Up> &poly) const {
                real_type res = 0;
                for (int i = 0; i < poly.size(); i++) {
                    res += _signed_area_with_segment(poly[i], poly[poly.next(i)]);
                }
                return std::abs(res);
            }

            /**
             * @brief 线段落在圆盘内部的长度；圆周算作内部边界。
             */
            template <typename Up>
            real_type length_inside_segment(const Line<Up> &seg) const {
                if (sgn(seg.norm2()) == 0 || sgn(r) == 0) return 0;
                auto ts = _segment_params(seg.s, seg.t);
                real_type res = 0, len = seg.length();
                for (int i = 0; i + 1 < (int)ts.size(); i++) {
                    real_type l = ts[i], rr = ts[i + 1];
                    if (sgn(rr - l) == 0) continue;
                    Point<real_type> mid = _point_at(seg.s, seg.t, (l + rr) / 2);
                    if (sgn(distance(mid, o) - r) <= 0) res += len * (rr - l);
                }
                return res;
            }

            /**
             * @brief 多边形边界落在圆盘内部的总长度。
             */
            template <typename Up>
            real_type length_inside_polygon(const Polygon<Up> &poly) const {
                real_type res = 0;
                for (int i = 0; i < poly.size(); i++) {
                    res += length_inside_segment(Line<Up>(poly[i], poly[poly.next(i)]));
                }
                return res;
            }

            /**
             * @brief 当前圆的圆周落在 rhs 圆盘内部的弧长。
             */
            template <typename Up>
            real_type arc_length_inside(const Circle<Up> &rhs) const {
                if (sgn(r) == 0) return 0;
                real_type d = distance(o, rhs.o);
                if (sgn(d) == 0) return sgn(r - rhs.r) <= 0 ? perimeter() : 0;
                if (sgn(d + r - rhs.r) <= 0) return perimeter();
                if (sgn(d - r - rhs.r) >= 0) return 0;
                if (sgn(d + rhs.r - r) <= 0) return 0;
                real_type x = _clamp_cos((r * r + d * d - rhs.r * rhs.r) / (2 * r * d));
                return 2 * r * std::acos(x);
            }

        private:
            static real_type pi() { return std::acos(real_type(-1)); }
            static real_type _clamp_cos(real_type x) {
                return std::clamp(x, real_type(-1), real_type(1));
            }

            static bool _same_point(const Point<real_type> &a, const Point<real_type> &b) {
                return sgn(a.x - b.x) == 0 && sgn(a.y - b.y) == 0;
            }

            static void _push_unique(std::vector<Point<real_type>> &ps, const Point<real_type> &p) {
                for (auto &q : ps) {
                    if (_same_point(p, q)) return;
                }
                ps.push_back(p);
            }

            static void _unique_params(std::vector<real_type> &ts) {
                std::sort(ts.begin(), ts.end());
                ts.erase(std::unique(ts.begin(), ts.end(), [](real_type a, real_type b) {
                    return sgn(a - b) == 0;
                }), ts.end());
            }

            template <typename Up, typename Vp>
            Point<real_type> _point_at(const Point<Up> &a, const Point<Vp> &b, real_type t) const {
                Point<real_type> u = a.template cast<real_type>();
                Point<real_type> v = b.template cast<real_type>();
                return u + (v - u) * t;
            }

            template <typename Up, typename Vp>
            std::vector<real_type> _segment_params(const Point<Up> &a, const Point<Vp> &b) const {
                std::vector<real_type> ts{0, 1};
                Point<real_type> u = a.template cast<real_type>() - o.template cast<real_type>();
                Point<real_type> v = b.template cast<real_type>() - o.template cast<real_type>();
                Point<real_type> d = v - u;
                real_type A = d.norm2();
                if (sgn(A) == 0) return ts;
                real_type B = 2 * dot(u, d);
                real_type C = u.norm2() - r * r;
                real_type delta = B * B - 4 * A * C;
                if (sgn(delta) < 0) return ts;
                delta = std::max(real_type(0), delta);
                real_type s = std::sqrt(delta);
                real_type t1 = (-B - s) / (2 * A);
                real_type t2 = (-B + s) / (2 * A);
                if (sgn(t1) >= 0 && sgn(t1 - 1) <= 0) ts.push_back(std::clamp(t1, real_type(0), real_type(1)));
                if (sgn(t2) >= 0 && sgn(t2 - 1) <= 0) ts.push_back(std::clamp(t2, real_type(0), real_type(1)));
                _unique_params(ts);
                return ts;
            }

            static real_type _sector_area(const Point<real_type> &a, const Point<real_type> &b, real_type r) {
                return r * r * std::atan2(cross(a, b), dot(a, b)) / 2;
            }

            template <typename Up, typename Vp>
            real_type _signed_area_with_segment(const Point<Up> &a, const Point<Vp> &b) const {
                if (sgn(r) == 0) return 0;
                auto ts = _segment_params(a, b);
                real_type res = 0;
                for (int i = 0; i + 1 < (int)ts.size(); i++) {
                    real_type l = ts[i], rr = ts[i + 1];
                    if (sgn(rr - l) == 0) continue;
                    Point<real_type> u = _point_at(a, b, l) - o.template cast<real_type>();
                    Point<real_type> v = _point_at(a, b, rr) - o.template cast<real_type>();
                    Point<real_type> mid = (u + v) / 2;
                    if (sgn(mid.norm() - r) <= 0) {
                        res += cross(u, v) / 2;
                    } else {
                        res += _sector_area(u, v, r);
                    }
                }
                return res;
            }
        };

        /**
         * @brief 以 ab 为直径的圆。
         */
        template <typename Tp, typename Up>
        Circle<real_type> circle_from_diameter(const Point<Tp> &a, const Point<Up> &b) {
            Point<real_type> o(((real_type)a.x + b.x) / 2, ((real_type)a.y + b.y) / 2);
            return Circle<real_type>(o, distance(o, a));
        }

        /**
         * @brief 三点外接圆；调用前需保证三点不共线。
         */
        template <typename Tp, typename Up, typename Vp>
        Circle<real_type> circumcircle(const Point<Tp> &a, const Point<Up> &b, const Point<Vp> &c) {
            real_type x1 = (real_type)b.x - a.x, y1 = (real_type)b.y - a.y;
            real_type x2 = (real_type)c.x - a.x, y2 = (real_type)c.y - a.y;
            real_type z1 = x1 * x1 + y1 * y1;
            real_type z2 = x2 * x2 + y2 * y2;
            real_type d = 2 * (x1 * y2 - y1 * x2);
            Point<real_type> o((real_type)a.x + (y2 * z1 - y1 * z2) / d,
                               (real_type)a.y + (x1 * z2 - x2 * z1) / d);
            return Circle<real_type>(o, distance(o, a));
        }

        /**
         * @brief 最小圆覆盖；randomized 为 true 时会先随机打乱点序。
         * @note 默认保持确定性顺序；随机化后为随机增量算法，期望复杂度 O(n)。
         */
        template <typename Tp>
        Circle<real_type> minimum_enclosing_circle(std::vector<Point<Tp>> points, bool randomized = false) {
            if (points.empty()) return Circle<real_type>();
            if (randomized) {
                static std::mt19937 rng((unsigned)std::chrono::steady_clock::now().time_since_epoch().count());
                std::shuffle(points.begin(), points.end(), rng);
            }
            Circle<real_type> c(points[0].template cast<real_type>(), 0);
            for (int i = 1; i < (int)points.size(); i++) {
                if (c.contains(points[i])) continue;
                c = Circle<real_type>(points[i].template cast<real_type>(), 0);
                for (int j = 0; j < i; j++) {
                    if (c.contains(points[j])) continue;
                    c = circle_from_diameter(points[i], points[j]);
                    for (int k = 0; k < j; k++) {
                        if (c.contains(points[k])) continue;
                        c = circumcircle(points[i], points[j], points[k]);
                    }
                }
            }
            return c;
        }
    }

    template <typename Tp>
    using Circle = GEO::Circle<Tp>;
}

#endif

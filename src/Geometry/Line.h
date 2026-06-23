#ifndef __OY_GEO_LINE__
#define __OY_GEO_LINE__

#include "Point.h"

#include <algorithm>
#include <vector>

namespace OY {
    namespace GEO {
        /**
         * @brief 两线段相交结果。
         * @note type: 0 不相交，1 规范相交，2 重叠成线段，3 端点相交。
         * @note type 为 1/3 时 p1 == p2；type 为 2 时 [p1, p2] 是公共线段。
         */
        struct SegmentIntersection {
            int type;
            Point<real_type> p1, p2;

            explicit operator bool() const { return type != 0; }
        };

        /**
         * @brief 二维直线/线段，由两个端点 s, t 表示。
         * @note 当作直线还是线段使用取决于调用的函数；方向为 s -> t。
         */
        template <typename Tp>
        struct Line {
            using value_type = Tp;
            using point_type = Point<Tp>;
            point_type s, t;

            constexpr Line() : s(), t() {}
            constexpr Line(const point_type &s, const point_type &t) : s(s), t(t) {}

            template <typename Up> constexpr operator Line<Up>() const { return Line<Up>(s.template cast<Up>(), t.template cast<Up>()); }
            template <typename Up> constexpr Line<Up> cast() const { return Line<Up>(s.template cast<Up>(), t.template cast<Up>()); }

            // 方向向量和长度；norm2 不开方，适合整数坐标下比较距离。
            constexpr auto vec() const -> decltype(t - s) { return t - s; }
            constexpr auto norm2() const -> decltype(vec().norm2()) { return vec().norm2(); }
            real_type norm() const { return vec().norm(); }
            real_type length() const { return norm(); }

            /**
             * @brief 点 p 与有向直线 s->t 的关系：1 左侧，-1 右侧，0 在线上。
             */
            template <typename Up>
            int relation(const Point<Up> &p) const { return sgn(cross(t - s, p - s)); }
            int relation(const point_type &p) const { return relation<Tp>(p); }

            /**
             * @brief 点 p 是否在无限直线 s-t 上。
             */
            template <typename Up>
            bool point_on_line(const Point<Up> &p) const { return relation(p) == 0; }
            bool point_on_line(const point_type &p) const { return point_on_line<Tp>(p); }

            /**
             * @brief 点 p 是否在线段 [s, t] 上，包含端点。
             */
            template <typename Up>
            bool point_on_segment(const Point<Up> &p) const { return point_on_line(p) && sgn(dot(p - s, p - t)) <= 0; }
            bool point_on_segment(const point_type &p) const { return point_on_segment<Tp>(p); }

            /**
             * @brief 点 p 是否在线段内部，不包含端点。
             */
            template <typename Up>
            bool point_on_segment_strict(const Point<Up> &p) const { return point_on_line(p) && sgn(dot(p - s, p - t)) < 0; }
            bool point_on_segment_strict(const point_type &p) const { return point_on_segment_strict<Tp>(p); }

            /**
             * @brief 点 p 到直线的投影点。
             * @note 按无限直线计算，返回点不一定在线段 [s, t] 内；需要保证 s != t。
             */
            template <typename Up>
            Point<real_type> projection(const Point<Up> &p) const {
                auto v = vec();
                real_type k = (real_type)dot(p - s, v) / (real_type)dot(v, v);
                return Point<real_type>((real_type)s.x + (real_type)v.x * k,
                                        (real_type)s.y + (real_type)v.y * k);
            }
            Point<real_type> projection(const point_type &p) const { return projection<Tp>(p); }

            /**
             * @brief 点 p 关于直线的对称点。
             * @note 对称轴为无限直线 s-t；需要保证 s != t。
             */
            template <typename Up>
            Point<real_type> reflection(const Point<Up> &p) const {
                auto q = projection(p);
                return q * 2 - p;
            }
            Point<real_type> reflection(const point_type &p) const { return reflection<Tp>(p); }

            /**
             * @brief 点 p 到直线的距离。
             * @note 需要保证 s != t。
             */
            template <typename Up>
            real_type distance_to_line(const Point<Up> &p) const {
                return std::abs((real_type)cross(vec(), p - s)) / norm();
            }
            real_type distance_to_line(const point_type &p) const { return distance_to_line<Tp>(p); }

            /**
             * @brief 点 p 到线段 [s, t] 的距离。
             * @note 允许退化线段；若 s == t，退化为点到 s 的距离。
             */
            template <typename Up>
            real_type distance_to_segment(const Point<Up> &p) const {
                auto v = vec();
                if (sgn(v.norm2()) == 0) return distance(p, s);
                if (sgn(dot(p - s, v)) < 0) return distance(p, s);
                if (sgn(dot(p - t, -v)) < 0) return distance(p, t);
                return distance_to_line(p);
            }
            real_type distance_to_segment(const point_type &p) const { return distance_to_segment<Tp>(p); }

            /**
             * @brief 两点是否在有向直线 s->t 的同侧；点在线上也视为同侧。
             */
            template <typename Up>
            bool same_side(const Point<Up> &a, const Point<Up> &b) const { return relation(a) * relation(b) >= 0; }
            bool same_side(const point_type &a, const point_type &b) const { return same_side<Tp>(a, b); }

            /**
             * @brief 直线沿左法向量平移 d；若要向右平移，传入 -d。
             * @note 需要保证 s != t。
             */
            Line<real_type> translated(real_type d) const {
                auto unit = rot90(vec()).unit();
                return Line<real_type>(s + unit * d, t + unit * d);
            }

            /**
             * @brief 两条直线是否平行；浮点坐标会按 EPS 判断。
             */
            template <typename Up>
            bool is_parallel(const Line<Up> &rhs) const { return sgn(cross(vec(), rhs.vec())) == 0; }

            /**
             * @brief 两条直线是否垂直；浮点坐标会按 EPS 判断。
             */
            template <typename Up>
            bool is_perpendicular(const Line<Up> &rhs) const { return sgn(dot(vec(), rhs.vec())) == 0; }

            /**
             * @brief 两条直线的交点；调用前需保证两直线不平行。
             */
            template <typename Up>
            Point<real_type> line_intersection(const Line<Up> &rhs) const {
                auto u = vec();
                auto v = rhs.vec();
                real_type k = (real_type)cross(rhs.s - s, v) / (real_type)cross(u, v);
                return Point<real_type>((real_type)s.x + (real_type)u.x * k,
                                        (real_type)s.y + (real_type)u.y * k);
            }

            /**
             * @brief 判断两条闭线段是否相交；只需要布尔结果时用这个。
             */
            template <typename Up>
            bool segment_intersects(const Line<Up> &rhs) const {
                if (!_box_intersects(rhs)) return false;
                return relation(rhs.s) * relation(rhs.t) <= 0 &&
                       rhs.relation(s) * rhs.relation(t) <= 0;
            }

            /**
             * @brief 两线段相交详情；重叠时 p1, p2 为重叠线段两端点。
             * @note 规范相交和端点相交都会返回单点，此时 p1 == p2。
             */
            template <typename Up>
            SegmentIntersection segment_intersection(const Line<Up> &rhs) const {
                if (!segment_intersects(rhs)) return {0, {}, {}};
                int d1 = relation(rhs.s), d2 = relation(rhs.t);
                int d3 = rhs.relation(s), d4 = rhs.relation(t);
                if (d1 == 0 && d2 == 0) {
                    return _collinear_intersection(rhs);
                }
                auto p = line_intersection(rhs);
                int type = d1 * d2 < 0 && d3 * d4 < 0 ? 1 : 3;
                return {type, p, p};
            }

            /**
             * @brief 两条闭线段最短距离。
             */
            template <typename Up>
            real_type distance_to_segment(const Line<Up> &rhs) const {
                if (segment_intersects(rhs)) return 0;
                return std::min({distance_to_segment(rhs.s), distance_to_segment(rhs.t),
                                 rhs.distance_to_segment(s), rhs.distance_to_segment(t)});
            }

        private:
            // 包围盒快速排除，减少线段相交时的方向判断。
            template <typename Up>
            bool _box_intersects(const Line<Up> &rhs) const {
                return _range_intersects(s.x, t.x, rhs.s.x, rhs.t.x) &&
                       _range_intersects(s.y, t.y, rhs.s.y, rhs.t.y);
            }

            // 一维区间是否相交；使用 calc_type 统一不同坐标类型。
            template <typename A, typename B, typename C, typename D>
            static bool _range_intersects(A a, B b, C c, D d) {
                auto l1 = std::min(a, b), r1 = std::max(a, b);
                auto l2 = std::min(c, d), r2 = std::max(c, d);
                using res_type = calc_type<decltype(l1), decltype(l2)>;
                return sgn(res_type(l1) - r2) <= 0 && sgn(res_type(l2) - r1) <= 0;
            }

            static bool _same_point(const Point<real_type> &a, const Point<real_type> &b) {
                return sgn(a.x - b.x) == 0 && sgn(a.y - b.y) == 0;
            }

            // 共线线段的公共部分：枚举四个端点，保留同时落在两段上的点。
            template <typename Up>
            SegmentIntersection _collinear_intersection(const Line<Up> &rhs) const {
                std::vector<Point<real_type>> ps;
                auto add = [&](const auto &p) {
                    Point<real_type> q = p.template cast<real_type>();
                    if (point_on_segment(q) && rhs.point_on_segment(q)) {
                        ps.push_back(q);
                    }
                };
                add(s), add(t), add(rhs.s), add(rhs.t);
                std::sort(ps.begin(), ps.end(), [](const auto &a, const auto &b) {
                    if (sgn(a.x - b.x) != 0) return a.x < b.x;
                    return a.y < b.y;
                });
                ps.erase(std::unique(ps.begin(), ps.end(), _same_point), ps.end());
                if (ps.empty()) return {0, {}, {}};
                if (ps.size() == 1) return {3, ps[0], ps[0]};
                return {2, ps.front(), ps.back()};
            }
        };

        /**
         * @brief 两条直线是否平行；不区分平行和重合。
         */
        template <typename Tp, typename Up>
        bool parallel(const Line<Tp> &a, const Line<Up> &b) { return a.is_parallel(b); }

        /**
         * @brief 两条直线是否垂直。
         */
        template <typename Tp, typename Up>
        bool perpendicular(const Line<Tp> &a, const Line<Up> &b) { return a.is_perpendicular(b); }

        /**
         * @brief 两条直线的交点；调用前需保证两直线不平行。
         */
        template <typename Tp, typename Up>
        Point<real_type> line_intersection(const Line<Tp> &a, const Line<Up> &b) { return a.line_intersection(b); }

        /**
         * @brief 两条闭线段相交详情；返回类型详见 SegmentIntersection。
         */
        template <typename Tp, typename Up>
        SegmentIntersection segment_intersection(const Line<Tp> &a, const Line<Up> &b) { return a.segment_intersection(b); }

        /**
         * @brief 点到直线的距离；点到线段距离请调用 l.distance_to_segment(p)。
         */
        template <typename Tp, typename Up>
        real_type distance(const Point<Tp> &p, const Line<Up> &l) { return l.distance_to_line(p); }

        /**
         * @brief 两条闭线段的距离。
         */
        template <typename Tp, typename Up>
        real_type distance(const Line<Tp> &a, const Line<Up> &b) { return a.distance_to_segment(b); }
    }

    template <typename Tp>
    using Line = GEO::Line<Tp>;
}

#endif

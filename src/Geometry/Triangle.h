#ifndef __OY_GEO_TRIANGLE__
#define __OY_GEO_TRIANGLE__

#include "Circle.h"

#include <cmath>

namespace OY {
    namespace GEO {
        /**
         * @brief 二维三角形，顶点为 a, b, c。
         */
        template <typename Tp>
        struct Triangle {
            using value_type = Tp;
            using point_type = Point<Tp>;
            point_type a, b, c;

            constexpr Triangle() : a(), b(), c() {}
            constexpr Triangle(const point_type &a, const point_type &b, const point_type &c) : a(a), b(b), c(c) {}

            template <typename Up>
            constexpr Triangle<Up> cast() const {
                return Triangle<Up>(a.template cast<Up>(), b.template cast<Up>(), c.template cast<Up>());
            }

            point_type &operator[](int i) { return i == 0 ? a : i == 1 ? b : c; }
            const point_type &operator[](int i) const { return i == 0 ? a : i == 1 ? b : c; }

            /**
             * @brief 有向面积的两倍；a->b->c 逆时针为正。
             */
            calc_type<Tp, Tp> signed_area2() const { return cross(b - a, c - a); }

            /**
             * @brief 三角形是否退化为共线三点。
             */
            bool is_degenerate() const { return sgn(signed_area2()) == 0; }

            real_type area() const { return std::abs((real_type)signed_area2()) / 2; }
            real_type perimeter() const { return side_a() + side_b() + side_c(); }

            // side_a 是顶点 a 的对边长度，其余同理。
            real_type side_a() const { return distance(b, c); }
            real_type side_b() const { return distance(c, a); }
            real_type side_c() const { return distance(a, b); }

            /**
             * @brief 点 p 与三角形的位置关系：0 外部，1 内部，2 边界。
             * @note 退化三角形只有边界没有内部；三条边线段的并集视为边界。
             */
            template <typename Up>
            int relation(const Point<Up> &p) const {
                if (is_degenerate()) {
                    return Line<Tp>(a, b).point_on_segment(p) ||
                                   Line<Tp>(b, c).point_on_segment(p) ||
                                   Line<Tp>(c, a).point_on_segment(p) ?
                           2 : 0;
                }
                int d1 = sgn(cross(b - a, p - a));
                int d2 = sgn(cross(c - b, p - b));
                int d3 = sgn(cross(a - c, p - c));
                bool has_pos = d1 > 0 || d2 > 0 || d3 > 0;
                bool has_neg = d1 < 0 || d2 < 0 || d3 < 0;
                if (has_pos && has_neg) return 0;
                return d1 == 0 || d2 == 0 || d3 == 0 ? 2 : 1;
            }

            /**
             * @brief 点 p 是否在三角形内；boundary 为 true 时包含边界。
             */
            template <typename Up>
            bool contains(const Point<Up> &p, bool boundary = true) const {
                int r = relation(p);
                return r == 1 || (boundary && r == 2);
            }

            /**
             * @brief 重心，三条中线交点。
             */
            Point<real_type> centroid() const {
                return Point<real_type>(((real_type)a.x + b.x + c.x) / 3, ((real_type)a.y + b.y + c.y) / 3);
            }

            /**
             * @brief 外心；调用前需保证三点不共线。
             */
            Point<real_type> circumcenter() const { return GEO::circumcircle(a, b, c).o; }

            /**
             * @brief 外接圆；调用前需保证三点不共线。
             */
            Circle<real_type> circumcircle() const { return GEO::circumcircle(a, b, c); }

            /**
             * @brief 内心，三条内角平分线交点。
             * @note 使用三边长加权，避免角度跨越 -pi/pi 时的误差。
             * @note 调用前需保证三角形非退化。
             */
            Point<real_type> incenter() const {
                real_type x = side_a(), y = side_b(), z = side_c();
                real_type s = x + y + z;
                return (a.template cast<real_type>() * x + b.template cast<real_type>() * y +
                        c.template cast<real_type>() * z) / s;
            }

            /**
             * @brief 内切圆；调用前需保证三角形非退化。
             */
            Circle<real_type> incircle() const {
                Point<real_type> o = incenter();
                return Circle<real_type>(o, Line<Tp>(a, b).distance_to_line(o));
            }

            /**
             * @brief 垂心，三条高线交点；调用前需保证三点不共线。
             */
            Point<real_type> orthocenter() const {
                Point<real_type> o = circumcenter();
                return a.template cast<real_type>() + b.template cast<real_type>() +
                       c.template cast<real_type>() - o * 2;
            }

            /**
             * @brief 费马点，到三个顶点距离和最小的点。
             * @note 若存在不小于 120 度的角，费马点就是该角顶点。
             * @note 三点共线时退化为一维中位点。
             */
            Point<real_type> fermat_point() const {
                static const real_type pi = std::acos(real_type(-1));
                if (is_degenerate()) {
                    if (sgn(dot(a - b, a - c)) <= 0) return a.template cast<real_type>();
                    if (sgn(dot(b - a, b - c)) <= 0) return b.template cast<real_type>();
                    return c.template cast<real_type>();
                }
                real_type A = angle(b - a, c - a);
                real_type B = angle(a - b, c - b);
                real_type C = angle(a - c, b - c);
                real_type limit = 2 * pi / 3;
                if (sgn(A - limit) >= 0) return a.template cast<real_type>();
                if (sgn(B - limit) >= 0) return b.template cast<real_type>();
                if (sgn(C - limit) >= 0) return c.template cast<real_type>();
                real_type wa = 1 / std::sin(A + pi / 3);
                real_type wb = 1 / std::sin(B + pi / 3);
                real_type wc = 1 / std::sin(C + pi / 3);
                real_type s = wa + wb + wc;
                return (a.template cast<real_type>() * wa + b.template cast<real_type>() * wb +
                        c.template cast<real_type>() * wc) / s;
            }

            /**
             * @brief 转成普通多边形，便于复用 Polygon 中的面积、重心等工具。
             */
            Polygon<Tp> to_polygon() const { return Polygon<Tp>({a, b, c}); }
        };
    }

    template <typename Tp>
    using Triangle = GEO::Triangle<Tp>;
}

#endif

#ifndef __OY_GEO_HALFPLANE__
#define __OY_GEO_HALFPLANE__

#include "Convex.h"

#include <algorithm>
#include <cmath>
#include <deque>
#include <utility>
#include <vector>

namespace OY {
    namespace GEO {
        /**
         * @brief 半平面，默认保留有向直线 s->t 的左侧区域。
         * @note 边界默认属于半平面；这与 Line::relation(p) >= 0 保持一致。
         */
        template <typename Tp>
        struct HalfPlane {
            using value_type = Tp;
            using point_type = Point<Tp>;
            using line_type = Line<Tp>;

            line_type line;

            constexpr HalfPlane() : line() {}
            constexpr HalfPlane(const point_type &s, const point_type &t) : line(s, t) {}
            constexpr explicit HalfPlane(const line_type &line) : line(line) {}

            template <typename Up>
            constexpr HalfPlane<Up> cast() const { return HalfPlane<Up>(line.template cast<Up>()); }

            const point_type &s() const { return line.s; }
            const point_type &t() const { return line.t; }
            constexpr auto vec() const -> decltype(line.vec()) { return line.vec(); }
            constexpr auto norm2() const -> decltype(line.norm2()) { return line.norm2(); }
            real_type angle() const { return _direction_angle(*this); }

            /**
             * @brief 点 p 与半平面边界的关系：1 内侧，0 边界，-1 外侧。
             */
            template <typename Up>
            int relation(const Point<Up> &p) const { return line.relation(p); }

            /**
             * @brief 点 p 是否在半平面内；boundary 为 false 时不包含边界。
             */
            template <typename Up>
            bool contains(const Point<Up> &p, bool boundary = true) const {
                int r = relation(p);
                return boundary ? r >= 0 : r > 0;
            }

            /**
             * @brief 两个半平面的边界直线是否平行。
             */
            template <typename Up>
            bool is_parallel(const HalfPlane<Up> &rhs) const { return line.is_parallel(rhs.line); }

            /**
             * @brief 两个半平面的边界直线交点；调用前需保证边界不平行。
             */
            template <typename Up>
            Point<real_type> line_intersection(const HalfPlane<Up> &rhs) const { return line.line_intersection(rhs.line); }

        private:
            template <typename Up>
            friend real_type _direction_angle(const HalfPlane<Up> &h);
            template <typename Up, typename Vp>
            friend bool _same_direction(const HalfPlane<Up> &a, const HalfPlane<Vp> &b);
        };

        template <typename Tp>
        real_type _direction_angle(const HalfPlane<Tp> &h) {
            static const real_type pi = std::acos(real_type(-1));
            real_type ang = h.vec().arg();
            if (ang < 0) ang += 2 * pi;
            if (ang >= 2 * pi) ang -= 2 * pi;
            return ang;
        }

        template <typename Tp, typename Up>
        bool _same_direction(const HalfPlane<Tp> &a, const HalfPlane<Up> &b) {
            return a.is_parallel(b) && sgn(dot(a.vec(), b.vec())) > 0;
        }

        template <typename Tp>
        bool _same_point(const Point<Tp> &a, const Point<Tp> &b) {
            return sgn(a.x - b.x) == 0 && sgn(a.y - b.y) == 0;
        }

        template <typename Container>
        bool _bounded_halfplanes(const Container &hps) {
            static const real_type pi = std::acos(real_type(-1));
            std::vector<real_type> angs;
            angs.reserve(hps.size());
            for (auto &h : hps) {
                real_type ang = _direction_angle(h) + pi / 2;
                if (ang >= 2 * pi) ang -= 2 * pi;
                angs.push_back(ang);
            }
            std::sort(angs.begin(), angs.end());
            real_type max_gap = 0;
            for (int i = 0; i < (int)angs.size(); i++) {
                real_type nxt = i + 1 == (int)angs.size() ? angs[0] + 2 * pi : angs[i + 1];
                max_gap = std::max(max_gap, nxt - angs[i]);
            }
            return sgn(max_gap - pi) < 0;
        }

        template <typename Tp>
        std::vector<HalfPlane<Tp>> _unique_halfplanes(std::vector<HalfPlane<Tp>> hps) {
            std::sort(hps.begin(), hps.end(), [](const auto &a, const auto &b) {
                return _direction_angle(a) < _direction_angle(b);
            });
            std::vector<HalfPlane<Tp>> res;
            for (auto &h : hps) {
                if (sgn(h.norm2()) == 0) continue;
                if (res.empty() || !_same_direction(res.back(), h)) {
                    res.push_back(h);
                } else if (!h.contains(res.back().s())) {
                    // 同方向边界只保留更靠内的一条。
                    res.back() = h;
                }
            }
            return res;
        }

        /**
         * @brief 半平面交，返回交集边界顶点，按逆时针顺序排列。
         * @note 半平面均保留边界左侧；若交集为空、退化或无界，返回空多边形。
         * @note 若需要把无界区域截成有限多边形，请使用 half_plane_intersection_with_box。
         */
        template <typename Tp>
        Polygon<real_type> half_plane_intersection(std::vector<HalfPlane<Tp>> hps) {
            hps = _unique_halfplanes(std::move(hps));
            std::deque<HalfPlane<Tp>> q;
            std::deque<Point<real_type>> ps;
            for (auto &h : hps) {
                while (!ps.empty() && !h.contains(ps.back())) ps.pop_back(), q.pop_back();
                while (!ps.empty() && !h.contains(ps.front())) ps.pop_front(), q.pop_front();
                if (!q.empty()) {
                    if (q.back().is_parallel(h)) return Polygon<real_type>();
                    ps.push_back(q.back().line_intersection(h));
                }
                q.push_back(h);
            }
            while (!ps.empty() && !q.front().contains(ps.back())) ps.pop_back(), q.pop_back();
            while (!ps.empty() && !q.back().contains(ps.front())) ps.pop_front(), q.pop_front();
            if (q.size() < 3 || !_bounded_halfplanes(q) || q.back().is_parallel(q.front())) return Polygon<real_type>();
            ps.push_back(q.back().line_intersection(q.front()));

            std::vector<Point<real_type>> res;
            res.reserve(ps.size());
            for (auto &p : ps) {
                if (res.empty() || !_same_point(res.back(), p)) res.push_back(p);
            }
            if (res.size() > 1 && _same_point(res.front(), res.back())) res.pop_back();
            if (res.size() < 3 || sgn(Polygon<real_type>(res).signed_area2()) == 0) return Polygon<real_type>();
            return Polygon<real_type>(res);
        }

        /**
         * @brief 大正方形边界半平面，常用于把无界半平面交截成可返回的有限多边形。
         */
        inline std::vector<HalfPlane<real_type>> bounding_box(real_type bound) {
            using P = Point<real_type>;
            std::vector<HalfPlane<real_type>> res;
            res.reserve(4);
            res.emplace_back(P(-bound, -bound), P(bound, -bound));
            res.emplace_back(P(bound, -bound), P(bound, bound));
            res.emplace_back(P(bound, bound), P(-bound, bound));
            res.emplace_back(P(-bound, bound), P(-bound, -bound));
            return res;
        }

        /**
         * @brief 半平面交并额外加入 [-bound, bound]^2 的边界。
         */
        template <typename Tp>
        Polygon<real_type> half_plane_intersection_with_box(const std::vector<HalfPlane<Tp>> &hps, real_type bound) {
            std::vector<HalfPlane<real_type>> all;
            all.reserve(hps.size() + 4);
            for (auto &h : hps) all.push_back(h.template cast<real_type>());
            auto box = bounding_box(bound);
            all.insert(all.end(), box.begin(), box.end());
            return half_plane_intersection(std::move(all));
        }

        /**
         * @brief 用半平面裁剪多边形，返回保留下来的部分。
         * @note 输入顶点需按边界顺序给出；多边形可以是顺时针或逆时针。
         */
        template <typename Tp, typename Up>
        Polygon<real_type> cut_polygon(const Polygon<Tp> &poly, const HalfPlane<Up> &h) {
            std::vector<Point<real_type>> res;
            auto push = [&](const Point<real_type> &p) {
                if (res.empty() || !_same_point(res.back(), p)) res.push_back(p);
            };
            for (int i = 0; i < poly.size(); i++) {
                const auto &a = poly[i];
                const auto &b = poly[poly.next(i)];
                bool in_a = h.contains(a), in_b = h.contains(b);
                if (in_a) push(a.template cast<real_type>());
                if (in_a != in_b) {
                    Line<Tp> e(a, b);
                    if (!e.is_parallel(h.line)) push(e.line_intersection(h.line));
                }
            }
            if (res.size() > 1 && _same_point(res.front(), res.back())) res.pop_back();
            if (res.size() < 3 || sgn(Polygon<real_type>(res).signed_area2()) == 0) return Polygon<real_type>();
            return Polygon<real_type>(res);
        }

        /**
         * @brief 把多边形每条边转成“保留内部”的半平面。
         * @note 自动识别顺/逆时针；面积为 0 时返回空数组。
         */
        template <typename Tp>
        std::vector<HalfPlane<Tp>> polygon_halfplanes(const Polygon<Tp> &poly) {
            std::vector<HalfPlane<Tp>> res;
            auto area2 = poly.signed_area2();
            if (poly.size() < 3 || sgn(area2) == 0) return res;
            res.reserve(poly.size());
            bool ccw = sgn(area2) > 0;
            for (int i = 0; i < poly.size(); i++) {
                int j = poly.next(i);
                if (ccw) res.emplace_back(poly[i], poly[j]);
                else res.emplace_back(poly[j], poly[i]);
            }
            return res;
        }

        /**
         * @brief 多边形的核；返回能看到整个多边形的点集。
         * @note 输入应为简单多边形，顶点顺/逆时针均可。
         */
        template <typename Tp>
        Polygon<real_type> polygon_kernel(const Polygon<Tp> &poly) {
            return half_plane_intersection(polygon_halfplanes(poly));
        }

        /**
         * @brief 两个凸多边形的交，返回交集多边形。
         * @note 输入顶点顺/逆时针均可；若输入非凸，多条边对应半平面不再等价于多边形内部。
         */
        template <typename Tp, typename Up>
        Polygon<real_type> convex_intersection(const Polygon<Tp> &a, const Polygon<Up> &b) {
            std::vector<HalfPlane<real_type>> hps;
            auto add = [&](const auto &poly) {
                auto hs = polygon_halfplanes(poly);
                for (auto &h : hs) hps.push_back(h.template cast<real_type>());
            };
            add(a), add(b);
            return half_plane_intersection(std::move(hps));
        }
    }

    template <typename Tp>
    using HalfPlane = GEO::HalfPlane<Tp>;
}

#endif

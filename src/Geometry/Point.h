#ifndef __OY_GEO_POINT__
#define __OY_GEO_POINT__

#include <algorithm>
#include <cmath>
#include <type_traits>

namespace OY {
    namespace GEO {
        using real_type = long double;

        inline constexpr real_type EPS = 1e-12L;

        template <typename Tp>
        constexpr real_type default_eps() { return std::is_floating_point<Tp>::value ? EPS : 0; }

        template <typename Tp, typename Up>
        using calc_type = std::conditional_t<
            std::is_floating_point<Tp>::value || std::is_floating_point<Up>::value,
            real_type,
            std::common_type_t<long long, Tp, Up>>;

        template <typename Tp>
        int sgn(Tp x, real_type eps = default_eps<Tp>()) {
            if constexpr (std::is_floating_point<Tp>::value) {
                if (x > eps) return 1;
                if (x < -eps) return -1;
                return 0;
            } else {
                return (Tp(0) < x) - (x < Tp(0));
            }
        }

        template <typename Tp>
        struct Point {
            using value_type = Tp;
            Tp x, y;

            constexpr Point() : x(), y() {}
            constexpr Point(const Tp &x, const Tp &y) : x(x), y(y) {}

            template <typename Up>
            constexpr operator Point<Up>() const { return Point<Up>(Up(x), Up(y)); }

            template <typename Up>
            constexpr Point<Up> cast() const { return Point<Up>(Up(x), Up(y)); }

            template <typename Up>
            constexpr Point &operator+=(const Point<Up> &rhs) { x += rhs.x, y += rhs.y; return *this; }
            template <typename Up>
            constexpr Point &operator-=(const Point<Up> &rhs) { x -= rhs.x, y -= rhs.y; return *this; }
            template <typename Up>
            constexpr Point &operator*=(const Up &k) { x *= k, y *= k; return *this; }
            template <typename Up>
            constexpr Point &operator/=(const Up &k) { x /= k, y /= k; return *this; }
            constexpr Point operator-() const { return Point(-x, -y); }
            constexpr Point rot90() const { return Point(-y, x); }
            // 向量长度平方；整数点会保持整数结果，常用于比较距离。
            constexpr calc_type<Tp, Tp> norm2() const {
                using res_type = calc_type<Tp, Tp>;
                return res_type(x) * x + res_type(y) * y;
            }
            // 向量长度，统一返回 real_type，避免整数除法误用。
            real_type norm() const { return std::hypot((real_type)x, (real_type)y); }
            real_type length() const { return norm(); }
            // 向量极角，范围为 [-pi, pi]。
            real_type arg() const { return std::atan2((real_type)y, (real_type)x); }
            // 单位向量，返回 real_type 坐标。
            Point<real_type> unit() const {
                real_type len = norm();
                return Point<real_type>(x / len, y / len);
            }
        };

        template <typename Tp, typename Up>
        constexpr Point<calc_type<Tp, Up>> operator+(const Point<Tp> &a, const Point<Up> &b) {
            using res_type = calc_type<Tp, Up>;
            return Point<res_type>(res_type(a.x) + b.x, res_type(a.y) + b.y);
        }
        template <typename Tp, typename Up>
        constexpr Point<calc_type<Tp, Up>> operator-(const Point<Tp> &a, const Point<Up> &b) {
            using res_type = calc_type<Tp, Up>;
            return Point<res_type>(res_type(a.x) - b.x, res_type(a.y) - b.y);
        }
        template <typename Tp, typename Up>
        constexpr Point<calc_type<Tp, Up>> operator*(const Point<Tp> &a, const Up &k) {
            using res_type = calc_type<Tp, Up>;
            return Point<res_type>(res_type(a.x) * k, res_type(a.y) * k);
        }
        template <typename Tp, typename Up>
        constexpr Point<calc_type<Tp, Up>> operator*(const Up &k, const Point<Tp> &a) {
            using res_type = calc_type<Tp, Up>;
            return Point<res_type>(res_type(k) * a.x, res_type(k) * a.y);
        }
        template <typename Tp, typename Up>
        constexpr auto operator/(const Point<Tp> &a, const Up &k)
            -> Point<decltype(a.x / k)> {
            using res_type = decltype(a.x / k);
            return Point<res_type>(a.x / k, a.y / k);
        }

        template <typename Tp, typename Up> constexpr bool operator==(const Point<Tp> &a, const Point<Up> &b) { return a.x == b.x && a.y == b.y; }
        template <typename Tp, typename Up> constexpr bool operator!=(const Point<Tp> &a, const Point<Up> &b) { return !(a == b); }
        template <typename Tp, typename Up> constexpr bool operator<(const Point<Tp> &a, const Point<Up> &b) { return a.x < b.x || (!(b.x < a.x) && a.y < b.y); }

        template <typename Istream, typename Tp> Istream &operator>>(Istream &is, Point<Tp> &p) { return is >> p.x >> p.y; }
        template <typename Ostream, typename Tp> Ostream &operator<<(Ostream &os, const Point<Tp> &p) { return os << '(' << p.x << ", " << p.y << ')'; }

        /**
         * @brief 点积。返回类型跟随坐标乘加结果，整数坐标会保持整数类型。
         */
        template <typename Tp, typename Up>
        constexpr calc_type<Tp, Up> dot(const Point<Tp> &a, const Point<Up> &b) {
            using res_type = calc_type<Tp, Up>;
            return res_type(a.x) * b.x + res_type(a.y) * b.y;
        }

        /**
         * @brief 叉积 `a x b`。正值表示 b 在 a 的逆时针方向。
         */
        template <typename Tp, typename Up>
        constexpr calc_type<Tp, Up> cross(const Point<Tp> &a, const Point<Up> &b) {
            using res_type = calc_type<Tp, Up>;
            return res_type(a.x) * b.y - res_type(a.y) * b.x;
        }

        template <typename Tp, typename Up>
        constexpr calc_type<Tp, Up> det(const Point<Tp> &a, const Point<Up> &b) { return cross(a, b); }

        template <typename Tp>
        constexpr calc_type<Tp, Tp> square(const Point<Tp> &p) { return p.norm2(); }

        template <typename Tp>
        real_type length(const Point<Tp> &p) { return p.norm(); }

        /**
         * @brief 两点欧氏距离，统一返回 real_type。
         */
        template <typename Tp, typename Up>
        real_type distance(const Point<Tp> &a, const Point<Up> &b) { return (a - b).norm(); }

        /**
         * @brief 向量逆时针旋转 90 度，坐标类型不变。
         */
        template <typename Tp>
        constexpr Point<Tp> rot90(const Point<Tp> &p) { return Point<Tp>(-p.y, p.x); }

        /**
         * @brief 向量逆时针旋转 theta 弧度，返回 real_type 点。
         */
        template <typename Tp, typename Up>
        Point<real_type> rotate(const Point<Tp> &p, Up theta) {
            real_type c = std::cos((real_type)theta);
            real_type s = std::sin((real_type)theta);
            return Point<real_type>(p.x * c - p.y * s, p.x * s + p.y * c);
        }

        /**
         * @brief 点 p 绕点 o 逆时针旋转 theta 弧度。
         */
        template <typename Tp, typename Up, typename Vp>
        Point<real_type> rotate_around(const Point<Tp> &p, const Point<Up> &o, Vp theta) { return rotate(p - o, theta) + o; }

        template <typename Tp>
        Point<real_type> normalize(const Point<Tp> &p) { return p.unit(); }

        /**
         * @brief 余弦定理，两向量夹角，范围为 [0, pi]。
         */
        template <typename Tp, typename Up>
        real_type angle(const Point<Tp> &a, const Point<Up> &b) {
            real_type den = a.norm() * b.norm();
            real_type val = (real_type)dot(a, b) / den;
            val = std::clamp(val, real_type(-1), real_type(1));
            return std::acos(val);
        }

        /**
         * @brief 角 abc 的大小，范围为 [0, pi]。
         */
        template <typename Tp, typename Up, typename Vp>
        real_type angle(const Point<Tp> &a, const Point<Up> &b, const Point<Vp> &c) { return angle(a - b, c - b); }
    }

    /**
     * @brief 短别名，和其他 OY 模板一样可直接写 `OY::Point<T>`。
     */
    template <typename Tp>
    using Point = GEO::Point<Tp>;
}

#endif

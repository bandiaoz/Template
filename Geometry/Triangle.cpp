#include "Line.cpp"

struct Triangle {
    Triangle(const Point &a, const Point &b, const Point &c)
        : a(a), b(b), c(c){};
    Point a, b, c;
    double getArea() { return det(b - a, c - a) * sin(arg_2(b - c, c - a)); }
    // 外心
    Point outCenter() {
        Line u, v;
        u.s = (a + b) / 2;
        u.t.x = u.s.x - a.y + b.y;
        u.t.y = u.s.y + a.x - b.x;
        v.s = (a + c) / 2;
        v.t.x = v.s.x - a.y + c.y;
        v.t.y = v.s.y + a.x - c.x;
        return lineIntersection(u, v);
    }
    // 内心
    Point inCenter() {
        Line u, v;
        u.s = a;
        double m = atan2(b.y - a.y, b.x - a.x);
        double n = atan2(c.y - a.y, c.x - a.x);
        u.t.x = u.s.x + cos((m + n) / 2);
        u.t.y = u.s.y + sin((m + n) / 2);
        v.s = b;
        m = atan2(a.y - b.y, a.x - b.x);
        n = atan2(c.y - b.y, c.x - b.x);
        v.t.x = v.s.x + cos((m + n) / 2);
        v.t.y = v.s.y + sin((m + n) / 2);
        return lineIntersection(u, v);
    }
    // 垂心
    Point perpenCenter() {
        Line u, v;
        u.s = c;
        u.t.x = u.s.x - a.y + b.y;
        u.t.y = u.s.y + a.x - b.x;
        v.s = b;
        v.t.x = v.s.x - a.y + c.y;
        v.t.y = v.s.y + a.x - c.x;
        return lineIntersection(u, v);
    }

    // 重心
    // 到三角形三顶点距离的平方和最小的点
    // 三角形内到三边距离之积最大的点
    Point baryCenter() {
        Line u((a + b) / 2, c), v((a + c) / 2, b);
        return lineIntersection(u, v);
    }

    // 费马点 到三角形三顶点距离之和最小的点
    Point fermentPoint() {
        if (arg_3(a, b, c) >= 2 * M_PI / 3) return b;
        if (arg_3(b, a, c) >= 2 * M_PI / 3) return a;
        if (arg_3(a, c, b) >= 2 * M_PI / 3) return c;
        Point ab = (a + b) / 2, ac = (a + c) / 2;
        Point z1 = sqrt(3.0) * (a - ab), z2 = sqrt(3.0) * (a - ac);
        z1 = rotate(z1, M_PI / 2);
        z2 = rotate(z2, M_PI / 2);
        if (arg_2(z1, c - ab) < M_PI / 2) {
            z1.x = -z1.x;
            z1.y = -z1.y;
        }
        if (arg_2(z2, b - ac) < M_PI / 2) {
            z2.x = -z2.x;
            z2.y = -z2.y;
        }
        return intersection(c, ab + z1, b, ac + z2);
    }
    // 模拟退火求费马点
    Point FermatPoint() {
        Point u, v;
        double step = std::abs(a.x) + std::abs(a.y) + std::abs(b.x) + std::abs(b.y) + std::abs(c.x) + std::abs(c.y);
        u = (a + b + c) / 3;
        while (step > 1e-10) {
            for (int k = 0; k < 10; step /= 2, ++k) {
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        v.x = u.x + step * i;
                        v.y = u.y + step * j;
                        if (dis(u, a) + dis(u, b) + dis(u, c) > dis(v, a) + dis(v, b) + dis(v, c)) {
                            u = v;
                        }
                    }
                }
            }
        }
        return u;
    }
};

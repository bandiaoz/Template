#include "PolygonAndConvex.cpp"

double sqr(double x) { return x * x; }
double mysqrt(double n) {
    return sqrt(max(0.0, n));
}  // 防止出现 sqrt(-eps) 的情况

struct Circle {
    Point o;
    double r;
    Circle(Point o = Point(), double r = 0) : o(o), r(r) {}
    bool operator==(const Circle &c) { return o == c.o && !sgn(r - c.r); }
    double area() { return PI * r * r; }
    double perimeter() { return r * PI * 2; }
    // 点在圆内，不包含边界
    bool pointIn(const Point &p) { return sgn((p - o).norm() - r) < 0; }
    // 判直线和圆相交，包括相切
    friend int isLineCircleIntersection(Line L, Circle c) {
        return L.disPointLine(c.o) < c.r + eps;
    }
    // 判线段和圆相交，包括端点和相切
    friend int isSegCircleIntersection(Line L, Circle c) {
        double t1 = dis(c.o, L.s) - c.r, t2 = dis(c.o, L.t) - c.r;
        Point t = c.o;
        if (t1 < eps || t2 < eps) return t1 > -eps || t2 > -eps;
        t.x += L.s.y - L.t.y;
        t.y += L.t.x - L.s.x;
        return det(L.s - t, c.o - t) * det(L.t - t, c.o - t) < eps && L.disPointLine(c.o) < c.r + eps;
    }
    // 判圆和圆相交，包括相切
    friend int isCirCirIntersection(Circle c1, Circle c2) {
        return dis(c1.o, c2.o) < c1.r + c2.r + eps &&
               dis(c1.o, c2.o) > fabs(c1.r - c2.r) - eps;
    }
    // 判圆和圆内含
    friend int isCirCirContain(Circle c1, Circle c2) {
        return sgn(dis(c1.o, c2.o) + min(c1.r, c2.r) - max(c1.r, c2.r)) <= 0;
    }
    // 计算圆上到点 p 最近点，如 p 与圆心重合，返回 p 本身
    friend Point dotPointCircle(Point p, Circle C) {
        Point u, v, c = C.o;
        if (dis(p, c) < eps) return p;
        u.x = c.x + C.r * fabs(c.x - p.x) / dis(c, p);
        u.y = c.y + C.r * fabs(c.y - p.y) / dis(c, p) * ((c.x - p.x) * (c.y - p.y) < 0 ? -1 : 1);
        v.x = c.x - C.r * fabs(c.x - p.x) / dis(c, p);
        v.y = c.y - C.r * fabs(c.y - p.y) / dis(c, p) * ((c.x - p.x) * (c.y - p.y) < 0 ? -1 : 1);
        return dis(u, p) < dis(v, p) ? u : v;
    }
    // 圆与线段交 用参数方程表示直线：P=A+t*(B-A)，带入圆的方程求解 t
    friend vector<Point> segCircleIntersection(const Line &l, const Circle &c) {
        double dx = l.t.x - l.s.x, dy = l.t.y - l.s.y;
        double A = dx * dx + dy * dy;
        double B = 2 * dx * (l.s.x - c.o.x) + 2 * dy * (l.s.y - c.o.y);
        double C = sqr(l.s.x - c.o.x) + sqr(l.s.y - c.o.y) - sqr(c.r);
        double delta = B * B - 4 * A * C;
        vector<Point> res;
        if (A < eps) return res;
        if (sgn(delta) >= 0) {  // or delta > -eps ?
            // 可能需要注意 delta 接近-eps 的情况，所以使用 mysqrt
            double w1 = (-B - mysqrt(delta)) / (2 * A);
            double w2 = (-B + mysqrt(delta)) / (2 * A);
            if (sgn(w1 - 1) <= 0 && sgn(w1) >= 0) {
                res.push_back(l.s + w1 * (l.t - l.s));
            }
            if (sgn(w2 - 1) <= 0 && sgn(w2) >= 0 && fabs(w1 - w2) > eps) {
                res.push_back(l.s + w2 * (l.t - l.s));
            }
        }
        return res;
    }
    // 圆与直线交
    friend vector<Point> lineCircleIntersection(const Line &l, const Circle &c) {
        double dx = l.t.x - l.s.x, dy = l.t.y - l.s.y;
        double A = dx * dx + dy * dy;
        double B = 2 * dx * (l.s.x - c.o.x) + 2 * dy * (l.s.y - c.o.y);
        double C = sqr(l.s.x - c.o.x) + sqr(l.s.y - c.o.y) - sqr(c.r);
        double delta = B * B - 4 * A * C;
        vector<Point> res;
        if (A < eps) return res;
        if (sgn(delta) >= 0) {  // or delta > -eps ?
            double w1 = (-B - mysqrt(delta)) / (2 * A);
            double w2 = (-B + mysqrt(delta)) / (2 * A);
            res.push_back(l.s + w1 * (l.t - l.s));
            if (fabs(w1 - w2) > eps) res.push_back(l.s + w2 * (l.t - l.s));
        }
        return res;
    }
    // 计算圆与圆的交点 保证圆不重合
    friend vector<Point> cirCirIntersection(Circle a, Circle b) {
        Point c1 = a.o;
        vector<Point> vec;
        if (dis(a.o, b.o) + eps > a.r + b.r &&
            dis(a.o, b.o) < fabs(a.r - b.r) + eps)
            return vec;
        Line L;
        double t = (1.0 + (sqr(a.r) - sqr(b.r)) / sqr(dis(a.o, b.o))) / 2;
        L.s = c1 + (b.o - a.o) * t;
        L.t.x = L.s.x + a.o.y - b.o.y;
        L.t.y = L.s.y - a.o.x + b.o.x;
        return lineCircleIntersection(L, a);
    }
    // 将向量 p 逆时针旋转 angle 角度
    // 求圆外一点对圆 (o,r) 的切点
    friend vector<Point> tangentPointCircle(Point poi, Circle C) {
        Point o = C.o;
        double r = C.r;
        vector<Point> vec;
        double dist = (poi - o).norm();
        if (dist < r - eps) return vec;
        if (fabs(dist - r) < eps) {
            vec.push_back(poi);
            return vec;
        }
        Point res1, res2;
        double line =
            sqrt((poi.x - o.x) * (poi.x - o.x) + (poi.y - o.y) * (poi.y - o.y));
        double angle = acos(r / line);
        Point unitVector, lin;
        lin.x = poi.x - o.x;
        lin.y = poi.y - o.y;
        unitVector.x = lin.x / sqrt(lin.x * lin.x + lin.y * lin.y) * r;
        unitVector.y = lin.y / sqrt(lin.x * lin.x + lin.y * lin.y) * r;
        res1 = rotate(unitVector, -angle) + o;
        res2 = rotate(unitVector, angle) + o;
        vec.push_back(res1);
        vec.push_back(res2);
        return vec;
    }
    // 扇形面积 a->b
    double sectorArea(const Point &a, const Point &b) const {
        double theta = atan2(a.y, a.x) - atan2(b.y, b.x);
        while (theta < 0) theta += 2 * PI;
        while (theta > 2.0 * PI) theta -= 2 * PI;
        theta = min(theta, 2.0 * PI - theta);
        return sgn(det(a, b)) * theta * r * r / 2.0;
    }
    // 与线段 AB 的交点计算面积 a->b
    double areaSegCircle(const Line &L) const {
        Point a = L.s, b = L.t;
        vector<Point> p = segCircleIntersection(Line(a, b), *this);
        bool ina = sgn((a - o).norm() - r) < 0;
        bool inb = sgn((b - o).norm() - r) < 0;
        if (ina) {
            if (inb)
                return det(a - o, b - o) / 2;
            else
                return det(a - o, p[0] - o) / 2 + sectorArea(p[0] - o, b - o);
        } else {
            if (inb)
                return det(p[0] - o, b - o) / 2 + sectorArea(a - o, p[0] - o);
            else {
                if (p.size() == 2)
                    return sectorArea(a - o, p[0] - o) +
                           sectorArea(p[1] - o, b - o) +
                           det(p[0] - o, p[1] - o) / 2;
                else
                    return sectorArea(a - o, b - o);
            }
        }
    }

    // 圆与多边形交，结果可以尝试 +eps
    friend double areaPolygonCircle(const Circle &c, const Polygon &a) {
        int n = a.p.size();

        double ans = 0;
        for (int i = 0; i < n; ++i) {
            if (sgn(det(a.p[i] - c.o, a.p[_next(i)] - c.o)) == 0) {
                continue;
            }
            ans += c.areaSegCircle((a.p[i], a.p[_next(i)]));
        }
        return ans;
    }
    // 两个圆的公共面积
    friend double areaCircleCircle(const Circle &A, const Circle &B) {
        double ans = 0.0;
        Circle M = (A.r > B.r) ? A : B;
        Circle N = (A.r > B.r) ? B : A;
        double D = dis(M.o, N.o);
        if ((D < M.r + N.r) && (D > M.r - N.r)) {
            double alpha = 2.0 * acos((M.r * M.r + D * D - N.r * N.r) / (2.0 * M.r * D));
            double beta = 2.0 * acos((N.r * N.r + D * D - M.r * M.r) / (2.0 * N.r * D));
            ans = (alpha / (2 * PI)) * M.area() + (beta / (2 * PI)) * N.area() -
                  0.5 * M.r * M.r * sin(alpha) - 0.5 * N.r * N.r * sin(beta);
        } else if (D <= M.r - N.r) {
            ans = N.area();
        }
        return ans;
    }

    // 三点求圆
    Circle getCircle3(const Point &p0, const Point &p1, const Point &p2) {
        double a1 = p1.x - p0.x, b1 = p1.y - p0.y, c1 = (a1 * a1 + b1 * b1) / 2;
        double a2 = p2.x - p0.x, b2 = p2.y - p0.y, c2 = (a2 * a2 + b2 * b2) / 2;
        double d = a1 * b2 - a2 * b1;
        Point o(p0.x + (c1 * b2 - c2 * b1) / d, p0.y + (a1 * c2 - a2 * c1) / d);
        return Circle(o, (o - p0).norm());
    }
    // 直径上两点求圆
    Circle getCircle2(const Point &p0, const Point &p1) {
        Point o((p0.x + p1.x) / 2, (p0.y + p1.y) / 2);
        return Circle(o, (o - p0).norm());
    }
    // 最小圆覆盖 用之前可以随机化 random_shuffle
    Circle minCirCover(vector<Point> &a) {
        int n = a.size();
        Circle c(a[0], 0);
        for (int i = 1; i < n; ++i) {
            if (!c.pointIn(a[i])) {
                c.o = a[i];
                c.r = 0;
                for (int j = 0; j < i; ++j) {
                    if (!c.pointIn(a[j])) {
                        c = getCircle2(a[i], a[j]);
                        for (int k = 0; k < j; ++k) {
                            if (!c.pointIn(a[k])) {
                                c = getCircle3(a[i], a[j], a[k]);
                            }
                        }
                    }
                }
            }
        }
        return c;
    }
    // 线段在圆内的长度
    friend double lengthSegInCircle(Line a, Circle c) {
        if (c.pointIn(a.s) && c.pointIn(a.t)) return a.norm();
        vector<Point> vec = segCircleIntersection(a, c);
        if (vec.size() == 0) return 0;
        if (vec.size() == 1) {
            if (c.pointIn(a.s)) return dis(vec[0], a.s);
            if (c.pointIn(a.t)) return dis(vec[0], a.t);
            return 0;
        }
        return dis(vec[0], vec[1]);
    }
    // 多边形在圆内的长度
    friend double lengthPolygonInCircle(Polygon a, Circle c) {
        double ans = 0;
        for (int i = 0; i < a.n; ++i) {
            Line li;
            li.s = a.p[i];
            li.t = a.p[(i + 1) % a.n];
            ans += lengthSegInCircle(li, c);
        }
        return ans;
    }
    // 圆 b 在圆 a 内的长度
    friend double lengthCircleInCircle(Circle a, Circle b) {
        if (a.r > b.r && a.r - b.r + eps > dis(a.o, b.o)) return b.perimeter();
        vector<Point> vec = cirCirIntersection(a, b);
        if (vec.size() < 2) return 0;
        // Line l1 = (vec[0], b.o), l2 = (vec[1], b.o);
        double ans = b.r * arg_3(vec[0], b.o, vec[1]);
        if (b.r >= a.r || !a.pointIn(b.o)) return b.r * ans;
        return b.perimeter() - ans;
    }
};

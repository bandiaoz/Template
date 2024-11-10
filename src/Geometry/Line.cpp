#include "Point.cpp"

struct Line {
    int id;
    Point s, t;
    Line(const Point &s = Point(), const Point &t = Point()) : s(s), t(t) {}

    Point vec() const { return t - s; }           // 化成矢量
    double norm() const { return vec().norm(); }  // 线段长度
    // 点是否在直线上
    bool pointOnLine(const Point &p) {            
        return sgn(det(p - s, t - s)) == 0;
    }
    // 点是否在线段上，含线段端点
    bool pointOnSeg(const Point &p) {  
        return pointOnLine(p) && sgn(dot(p - s, p - t)) <= 0;
    }
    // 点是否在线段上，不含线段端点
    bool pointOnSegInterval(const Point &p) {  
        return pointOnLine(p) && sgn(dot(p - s, p - t) < 0);
    }
    // 点到直线的垂足
    Point pedalPointLine(const Point &p) {  
        return s + vec() * ((dot(p - s, vec()) / norm()) / norm());
    }
    // 点到直线的距离
    double disPointLine(const Point &p) { 
        return std::abs(det(p - s, vec()) / norm());
    }
    // 点到线段的距离
    double disPointSeg(const Point &p) {  
        if (sgn(dot(p - s, t - s)) < 0) return (p - s).norm();
        if (sgn(dot(p - t, s - t)) < 0) return (p - t).norm();
        return disPointLine(p);
    }
    // 计算点 p 与直线的关系，返回 ONLINE、LEFT、RIGHT 上 0 左 1 右 -1
    int relation(const Point &p) { return sgn(det(t - s, p - s)); }
    // 判断 a, b 是否在直线的同侧或者同时在直线上
    bool sameSide(const Point &a, const Point &b) {
        return relation(a) == relation(b);
    }
    // 二维平面上点 p 关于直线的对称点
    Point symPoint(const Point &p) {
        return 2.0 * s - p + 2.0 * (t - s) * dot(p - s, t - s) / ((t.x - s.x) * (t.x - s.x) + (t.y - s.y) * (t.y - s.y));
    }
    // 判断两直线是否平行
    friend bool isParallel(const Line &l1, const Line &l2) {
        return sgn(det(l1.vec(), l2.vec())) == 0;
    }
    // 判断两直线是否垂直
    friend bool isVertical(const Line &l1, const Line &l2) {
        return sgn(dot(l1.vec(), l2.vec())) == 0;
    }
    // 利用相似三角形对应成比例求两直线的交点
    friend Point lineIntersection(const Line &l1, const Line &l2) {
        double s1 = det(l1.s - l2.s, l2.vec());
        double s2 = det(l1.t - l2.s, l2.vec());
        return (l1.t * s1 - l1.s * s2) / (s1 - s2);
    }
    // 求两直线交点的另一种方法
    friend Point getLineIntersection(const Line &u, const Line &v) {
        return u.s + (u.t - u.s) * det(u.s - v.s, v.s - v.t) /
                         det(u.s - u.t, v.s - v.t);
    }
    // 判断直线 l1 和线段 l2 是否相交
    friend bool isLineSegIntersection(Line l1, Line l2) {
        return l1.relation(l2.s) * l1.relation(l2.t) <= 0;
    }
    // 判断线段交，返回是否有交点
    friend bool isSegIntersection(Line l1, Line l2) {
        if (!sgn(det(l2.s - l1.s, l1.vec())) &&
            !sgn(det(l2.t - l1.t, l1.vec()))) {
            return l1.pointOnSeg(l2.s) || l1.pointOnSeg(l2.t) ||
                   l2.pointOnSeg(l1.s) || l2.pointOnSeg(l1.t);
        }
        return !l1.sameSide(l2.s, l2.t) && !l2.sameSide(l1.s, l1.t);
    }

    // 规范相交，两线段仅有一个非端点处的交点
    // 判断线段相交，并求线段交点，1 规范相交，2 相交，0 不交
    friend int segSegIntersection(Line l1, Line l2, Point &p) {
        Point a, b, c, d;
        a = l1.s;
        b = l1.t;
        c = l2.s;
        d = l2.t;
        double s1, s2, s3, s4;
        int d1, d2, d3, d4;
        d1 = sgn(s1 = det(b - a, c - a)); // l1.relation(l2.s);
        d2 = sgn(s2 = det(b - a, d - a)); // l1.relation(l2.t);
        d3 = sgn(s3 = det(d - c, a - c)); // l2.relation(l1.s);
        d4 = sgn(s4 = det(d - c, b - c)); // l2.relation(l1.t);

        // 若规范相交则求交点的代码
        if (d1 * d2 < 0 && d3 * d4 < 0) {
            p.x = (c.x * s2 - d.x * s1) / (s2 - s1);
            p.y = (c.y * s2 - d.y * s1) / (s2 - s1);
            return 1;
        }

        // 判断非规范相交
        // d1 == 0, 则证明 a, b, c 三点共线;
        // 如果 sgn(dot(a - c, b - c)) < 0, 则说明点 c 在点 a，b 的中间；
        // 如果 sgn(dot(a - c, b - c)) == 0，则说明点 c 与线段 ab 的端点 a，或者 b 重合。
        // 如果 sgn(dot(a - c, b - c)) > 0，则说明点 c 在线段 ab 的外面。
        if ((d1 == 0 && sgn(dot(a - c, b - c)) <= 0) ||
            (d2 == 0 && sgn(dot(a - d, b - d)) <= 0) ||
            (d3 == 0 && sgn(dot(c - a, d - a)) <= 0) ||
            (d4 == 0 && sgn(dot(c - b, d - b)) <= 0)) {
            return 2;
        }
        return 0;
    }

    // 直线沿法向量 (指向直线逆时针方向，若需要顺时针则移动 -d) 移动 d 距离
    friend Line move(const Line &l, const double &d) {
        Point t = l.vec();
        t = t / t.norm();
        t = rotate(t, M_PI / 2);
        return Line(l.s + t * d, l.t + t * d);
    }
    // 计算线段 l1 到线段 l2 的最短距离
    friend double disSegSeg(Line &l1, Line &l2) {
        double d1, d2, d3, d4;
        if (isSegIntersection(l1, l2))
            return 0;
        else {
            d1 = l2.disPointSeg(l1.s);
            d2 = l2.disPointSeg(l1.t);
            d3 = l1.disPointSeg(l2.s);
            d4 = l1.disPointSeg(l2.t);
            return std::min(std::min(d1, d2), std::min(d3, d4));
        }
    }
    // 两直线的夹角，返回 [0, PI] 弧度
    friend double argLineLine(Line l1, Line l2) {
        Point u = l1.vec();
        Point v = l2.vec();
        return acos(dot(u, v) / (u.norm() * v.norm()));
    }
};

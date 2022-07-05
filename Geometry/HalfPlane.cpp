#include "PolygonAndConvex.cpp"

const int inf = 1e9;

struct HalfPlane : public Line {  // 半平面
    // ax + by + c <= 0
    double a, b, c;
    // s->t 的左侧表示半平面
    HalfPlane(const Point &s = Point(), const Point &t = Point()) : Line(s, t) {
        a = t.y - s.y;
        b = s.x - t.x;
        c = det(t, s);
    }
    HalfPlane(double a, double b, double c) : a(a), b(b), c(c) {}
    // 求点p带入直线方程的值
    double calc(const Point &p) const { return p.x * a + p.y * b + c; }
    // 好像跟lineIntersection一样，那个是4个点计算。这个是用abc与两点进行计算
    friend Point halfxLine(const HalfPlane &h, const Line &l) {
        Point res;
        double t1 = h.calc(l.s), t2 = h.calc(l.t);
        res.x = (t2 * l.s.x - t1 * l.t.x) / (t2 - t1);
        res.y = (t2 * l.s.y - t1 * l.t.y) / (t2 - t1);
        return res;
    }
    // 用 abc 进行计算 尚未测试
    friend Point halfxHalf(const HalfPlane &h1, const HalfPlane &h2) {
        return Point(
            (h1.b * h2.c - h1.c * h2.b) / (h1.a * h2.b - h2.a * h1.b) + eps,
            (h1.a * h2.c - h2.a * h1.c) / (h1.b * h2.a - h1.a * h2.b) + eps);
    }
    // 凸多边形与半平面交(cut)
    friend Convex halfxConvex(const HalfPlane &h, const Convex &c) {
        Convex res;
        for (int i = 0; i < c.n; ++i) {
            if (h.calc(c.p[i]) < -eps)
                res.p.push_back(c.p[i]);
            else {
                int j = i - 1;
                if (j < 0) j = c.n - 1;
                if (h.calc(c.p[j]) < -eps)
                    res.p.push_back(halfxLine(h, Line(c.p[j], c.p[i])));
                j = i + 1;
                if (j == c.n) j = 0;
                if (h.calc(c.p[j]) < -eps) {
                    res.p.push_back(halfxLine(h, Line(c.p[i], c.p[j])));
                }
            }
        }
        res.n = res.p.size();
        return res;
    }
    // 点在半平面内
    friend int satisfy(const Point &p, const HalfPlane &h) {
        return sgn(det(p - h.s, h.t - h.s)) <= 0;
    }
    friend bool operator<(const HalfPlane &h1, const HalfPlane &h2) {
        int res = sgn(h1.vec().arg() - h2.vec().arg());
        return res == 0 ? satisfy(h1.s, h2) : res < 0;
    }
    // 半平面交出的凸多边形
    friend Convex halfx(vector<HalfPlane> &v) {
        sort(v.begin(), v.end());
        deque<HalfPlane> q;
        deque<Point> ans;
        q.push_back(v[0]);
        for (int i = 1; i < v.size(); ++i) {
            if (sgn(v[i].vec().arg() - v[i - 1].vec().arg()) == 0) continue;
            while (ans.size() > 0 && !satisfy(ans.back(), v[i])) {
                ans.pop_back();
                q.pop_back();
            }
            while (ans.size() > 0 && !satisfy(ans.front(), v[i])) {
                ans.pop_front();
                q.pop_front();
            }
            ans.push_back(lineIntersection(q.back(), v[i]));
            q.push_back(v[i]);
        }
        while (ans.size() > 0 && !satisfy(ans.back(), q.front())) {
            ans.pop_back();
            q.pop_back();
        }
        while (ans.size() > 0 && !satisfy(ans.front(), q.back())) {
            ans.pop_front();
            q.pop_front();
        }
        ans.push_back(lineIntersection(q.back(), q.front()));
        Convex c(ans.size());
        int i = 0;
        for (deque<Point>::iterator it = ans.begin(); it != ans.end();
             ++it, ++i) {
            c.p[i] = *it;
        }
        return c;
    }
};
// 多边形的核，逆时针
Convex core(const Polygon &a) {
    Convex res;
    res.p.push_back(Point(-inf, -inf));
    res.p.push_back(Point(inf, -inf));
    res.p.push_back(Point(inf, inf));
    res.p.push_back(Point(-inf, inf));
    res.n = 4;
    for (int i = 0; i < a.n; i++) {
        res = halfxConvex(HalfPlane(a.p[i], a.p[(i + 1) % a.n]), res);
    }
    return res;
}
// 凸多边形交出的凸多边形
Convex convexxConvex(Convex &c1, Convex &c2) {
    vector<HalfPlane> h;
    for (int i = 0; i < c1.p.size(); ++i)
        h.push_back(HalfPlane(c1.p[i], c1.p[(i + 1) % c1.p.size()]));
    for (int i = 0; i < c2.p.size(); i++)
        h.push_back(HalfPlane(c2.p[i], c2.p[(i + 1) % c2.p.size()]));
    return halfx(h);
}
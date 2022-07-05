#include "Line.cpp"

struct Polygon {
#define _next(i) ((i + 1) % n)
    int n;
    vector<Point> p;
    Polygon(vector<Point> &v) : p(v) { n = p.size(); }
    Polygon(int n = 0) : n(n) { p.resize(n); }
    // 多边形周长
    double perimeter() {
        double sum = 0;
        for (int i = 0; i < n; ++i) sum += (p[_next(i)] - p[i]).norm();
        return sum;
    }
    // 多边形面积
    double area() {
        double sum = 0;
        for (int i = 0; i < n; ++i) sum += det(p[i], p[_next(i)]);
        return fabs(sum) / 2;
    }  // eps
    // 判断点与多边形的位置关系 0外, 1内, 2边上
    int pointIn(const Point &t) {
        int num = 0;
        for (int i = 0; i < n; i++) {
            if (Line(p[i], p[_next(i)]).pointOnSeg(t)) return 2;
            int k = sgn(det(p[_next(i)] - p[i], t - p[i]));
            int d1 = sgn(p[i].y - t.y);
            int d2 = sgn(p[_next(i)].y - t.y);
            if (k > 0 && d1 <= 0 && d2 > 0) num++;
            if (k < 0 && d2 <= 0 && d1 > 0) num--;
        }
        return num % 2;
    }
    // 多边形重心
    Point baryCenter() {
        Point ans;
        if (sgn(area()) == 0) return ans;
        for (int i = 0; i < n; ++i)
            ans = ans + (p[i] + p[_next(i)]) * det(p[i], p[_next(i)]);
        return ans / area() / 6 + eps;  // 要加eps吗？
    }
    // 判断多边形是否为凸多边形 (需要已经排好序)
    bool isConvex() {  //不允许3点共线
        int s[3] = {1, 1, 1};
        for (int i = 0; i < n && (s[0] || s[2]) && s[1]; ++i) {
            s[1 + sgn(det(p[_next(i)] - p[i], p[_next(_next(i))] - p[i]))] = 0;
        }
        return (s[0] || s[2]) && s[1];
    }
    bool isConvex_3() {  // 允许3点共线
        int s[3] = {1, 1, 1};
        for (int i = 0; i < n && (s[0] || s[2]); ++i) {
            s[1 + sgn(det(p[_next(i)] - p[i], p[_next(_next(i))] - p[i]))] = 0;
        }
        return (s[0] || s[2]);
    }
    // 多边形边界上格点的数量
    long long borderPointNum() {
        long long num = 0;
        for (int i = 0; i < n; ++i) {
            num += gcd((long long)fabs(p[_next(i)].x - p[i].x),
                       (long long)fabs(p[_next(i)].y - p[i].y));
        }
        return num;
    }
    // 多边形内格点数量
    long long inSidePointNum() {
        return (long long)(area()) + 1 - borderPointNum() / 2;
    }
    // 点 p 在以 l1l2 为对角线的矩形内边界上
    inline int dotOnlineIn(Point p, Point l1, Point l2) {
        return sgn(det(p - l2, l1 - l2)) && (l1.x - p.x) * (l2.x - p.x) < eps &&
               (l1.y - p.y) * (l2.y - p.y) < eps;
    }
    // 判线段在任意多边形内,顶点按顺时针或逆时针给出,与边界相交返回1
    int insidePolygon(Line l) {
        vector<Point> t;
        Point tt, l1 = l.s, l2 = l.t;
        if (!pointIn(l.s) || !pointIn(l.t)) return 0;
        for (int i = 0; i < n; ++i) {
            if (l.sameSide(p[i], p[(i + 1) % n]) &&
                l.sameSide(p[i], p[(i + 1) % n]))
                return 0;
            else if (dotOnlineIn(l1, p[i], p[(i + 1) % n]))
                t.push_back(l1);
            else if (dotOnlineIn(l2, p[i], p[(i + 1) % n]))
                t.push_back(l2);
            else if (dotOnlineIn(p[i], l1, l2))
                t.push_back(p[i]);
        }
        for (int i = 0; i < t.size(); ++i) {
            for (int j = i + 1; j < t.size(); ++j) {
                if (!pointIn((t[i] + t[j]) / 2)) return 0;
            }
        }
        return 1;
    }
};

struct Convex : public Polygon {
    Convex(int n = 0) : Polygon(n) {}
    Convex(vector<Point> &a) { // 传入n个点构造凸包
        Convex res(a.size() * 2 + 7);
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());  // 去重点
        int m = 0;
        for (int i = 0; i < a.size(); ++i) {
            // <=0 则允许3点共线，<0 则不允许
            while (m > 1 && sgn(det(res.p[m - 1] - res.p[m - 2], a[i] - res.p[m - 2])) <= 0)
                m--;
            res.p[m++] = a[i];
        }
        int k = m;
        for (int i = a.size() - 2; i >= 0; --i) {
            while (m > k && sgn(det(res.p[m - 1] - res.p[m - 2], a[i] - res.p[m - 2])) <= 0) {
                m--;
            }
            res.p[m++] = a[i];
        }
        if (m > 1) m--;
        res.p.resize(m);
        res.n = m;
        *this = res;
    }

    // 需要先求凸包，若凸包每条边除端点外都有点，则可唯一确定凸包
    bool isUnique(vector<Point> &v) {
        if (sgn(area()) == 0) return 0;
        for (int i = 0; i < n; ++i) {
            Line l(p[i], p[_next(i)]);
            bool flag = 0;
            for (int j = 0; j < v.size(); ++j) {
                if (l.pointOnSegInterval(v[j])) {
                    flag = 1;
                    break;
                }
            }
            if (!flag) return 0;
        }
        return 1;
    }
    // O(n)时间内判断点是否在凸包内 包含边
    bool containon(const Point &a) {
        for (int sign = 0, i = 0; i < n; ++i) {
            int x = sgn(det(p[i] - a, p[_next(i)] - a));
            if (x == 0) continue;  // return 0; // 改成不包含边
            if (!sign)
                sign = x;
            else if (sign != x)
                return 0;
        }
        return 1;
    }
    // O(logn)时间内判断点是否在凸包内
    bool containologn(const Point &a) {
        Point g = (p[0] + p[n / 3] + p[2.0 * n / 3]) / 3.0;
        int l = 0, r = n;
        while (l + 1 < r) {
            int m = (l + r) >> 1;
            if (sgn(det(p[l] - g, p[m] - g)) > 0) {
                if (sgn(det(p[l] - g, a - g)) >= 0 &&
                    sgn(det(p[m] - g, a - g)) < 0)
                    r = m;
                else
                    l = m;
            } else {
                if (sgn(det(p[l] - g, a - g)) < 0 &&
                    sgn(det(p[m] - g, a - g)) >= 0)
                    l = m;
                else
                    r = m;
            }
        }
        return sgn(det(p[r % n] - a, p[l] - a)) - 1;
    }
    // 最远点对（直径）
    int fir, sec;  // 最远的两个点对应标号
    double diameter() {
        double mx = 0;
        if (n == 1) {
            fir = sec = 0;
            return mx;
        }
        for (int i = 0, j = 1; i < n; ++i) {
            while (sgn(det(p[_next(i)] - p[i], p[j] - p[i]) -
                       det(p[_next(i)] - p[i], p[_next(j)] - p[i])) < 0) {
                j = _next(j);
            }
            double d = dis(p[i], p[j]);
            if (d > mx) {
                mx = d;
                fir = i;
                sec = j;
            }
            d = dis(p[_next(i)], p[_next(j)]);
            if (d > mx) {
                mx = d;
                fir = _next(i);
                sec = _next(j);
            }
        }
        return mx;
    }

    // 凸包是否与直线有交点O(log(n)), 需要On的预处理, 适合判断与直线集是否有交点
    vector<double> ang;  // 角度
    bool isinitangle;
    int finda(const double &x) {
        return upper_bound(ang.begin(), ang.end(), x) - ang.begin();
    }
    double getAngle(const Point &p) {  // 获取向量角度[0, 2PI]
        double res = atan2(p.y, p.x);  // （-PI, PI】
        //      if (res < 0) res += 2 * pi; //为何不可以
        if (res < -PI / 2 + eps) res += 2 * PI;  // eps修正精度
        return res;
    }
    void initAngle() {
        for (int i = 0; i < n; ++i) {
            ang.push_back(getAngle(p[_next(i)] - p[i]));
        }
        isinitangle = 1;
    }
    bool isxLine(const Line &l) {
        if (!isinitangle) initAngle();
        int i = finda(getAngle(l.t - l.s));
        int j = finda(getAngle(l.s - l.t));
        if (sgn(det(l.t - l.s, p[i] - l.s) * det(l.t - l.s, p[j] - l.s) >= 0))
            return 0;
        return 1;
    }
};

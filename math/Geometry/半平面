typedef vector<Point> Polygon;

//用有向直线AB的左半平面切割 O(n)
Polygon CutPolygon(const Polygon &p, Point A, Point B) {
    Polygon ans;
    int n = p.size();
    for (int i = 0; i < n; ++i) {
        const Point &C = p[i], &D = p[(i + 1) % n];
        if (sgn((B - A) ^ (C - A)) >= 0) ans.push_back(C);
        if (sgn((B - A) ^ (C - D)) != 0) {
            double t = ((B - A) ^ (C - A)) / ((D - C) ^ (B - A));
            if (sgn(t) > 0 && sgn(t - 1) < 0) ans.push_back(C + (D - C) * t);
        }
    }
    return ans;
}

//半平面交
inline bool onleft(Line L, Point P) {
    return (L.v ^ (P - L.P)) > 0;
}
Point GetLineIntersection(Line A, Line B) {
    Point u = A.P - B.P;
    double t = (B.v ^ u) / (A.v ^ B.v);
    return A.point(t);
}
Point p[N];
Line q[N];
int HalfPlaneIntersection(Line *L, int n, Point *Poly) {
    sort(L, L + n);
    int top = 0, bot = 0;
    q[0] = L[0];
    for (int i = 1; i < n; ++i) {
        while (top < bot && !onleft(L[i], p[bot - 1])) --bot;
        while (top < bot && !onleft(L[i], p[top])) ++top;
        q[++bot] = L[i];
        if (sgn(L[i].v ^ q[bot - 1].v) == 0) {
            --bot;
            if (onleft(q[bot], L[i].P)) q[bot] = L[i];
        }
        if (top < bot) p[bot - 1] = GetLineIntersection(q[bot - 1], q[bot]);
    }
    while (top < bot && !onleft(q[top], p[bot - 1])) --bot;
    if (bot - top <= 1) return 0;
    p[bot] = GetLineIntersection(q[bot], q[top]);
    int m = 0;
    for (int i = top; i <= bot; ++i) Poly[m++] = p[i];
    return m;
}
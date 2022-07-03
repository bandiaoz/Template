#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <iomanip>
#define rep(i, a, n) for (int i = a; i <= n; ++i)
#define per(i, a, n) for (int i = n; i >= a; --i)
#ifdef LOCAL
// #include "Print.h"
#define de(...) W('[', #__VA_ARGS__,"] =", __VA_ARGS__)
#else
#define de(...)
#endif
using namespace std;
typedef long long ll;
const int maxn = 1e5 + 5;
struct point{
	int x, y;
	point(){}
	point(double _x, double _y): x(_x), y(_y) {}
	friend inline point operator -(const point &a, const point &b) { return point(a.x - b.x, a.y - b.y); }
	friend inline double operator *(const point &a, const point &b) { return a.x * b.y - a.y * b.x; }
	inline double dis() {return sqrt(x * x + y * y);}
} p[maxn], q[maxn];
int n, tot;
inline bool cmp(point a, point b) {
	double det = (a - p[1]) * (b - p[1]);
	if(det) return det < 0;
	return (a - p[1]).dis() < (b-p[1]).dis();
}
inline void graham() {
	int t = 1;
	rep(i, 2, n) if (p[i].x < p[t].x || (p[i].x == p[t].x && p[i].y < p[t].y)) t = i;
	swap(p[1], p[t]);
	sort(p + 2, p + n + 1, cmp);
	q[++tot] = p[1], q[++tot] = p[2];
	rep(i, 3, n) {
		while(tot > 1 && (p[i] - q[tot - 1]) * (q[tot] - q[tot-1]) <= 0) --tot;
		q[++tot] = p[i];
	}
	q[++tot] = q[1];
}
int L;
int solve() {
	scanf("%d", &n);
    scanf("%d", &L);
	rep(i, 1, n) scanf("%d%d", &p[i].x, &p[i].y);
	graham();
	double ans = 0;
	rep(i, 1, tot - 1) ans += (q[i + 1] - q[i]).dis();
    cout << fixed << setprecision(0) << ans + 2 * acos(-1) * L << endl;
	return 0;
}
int case_Test() {
    solve();
    return 0;
}
int main() {
#ifdef LOCAL
    freopen("/Users/chenjinglong/cpp_code/in.in", "r", stdin);
    freopen("/Users/chenjinglong/cpp_code/out.out", "w", stdout);
    clock_t start = clock();
#endif
    int _ = 1;
    // scanf("%d", &_);
    while (_--) case_Test();
#ifdef LOCAL
    printf("Time used: %.3lfs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
#endif
    return 0;
}
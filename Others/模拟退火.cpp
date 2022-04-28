#include <cmath>
#include <iostream>
using namespace std;
#define eps 1e-8
#define IO ios::sync_with_stdio(false), cin.tie(0)
//Gym - 101981D
const int maxn = 105;
int n;
struct point {
    double x, y, z;
} p[maxn];

double dis(point a, point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

double solve() {
    double step = 10000, ans = 1e30;
    point xx;
    xx.x = xx.y = xx.z = 0;
    int pos = 1;
    while (step > eps) {
        for (int i = 1; i <= n; ++i)  //找一个最远的点
            if (dis(xx, p[pos]) < dis(xx, p[i])) pos = i;
        double mt = dis(xx, p[pos]);
        ans = min(ans, mt);
        xx.x += (p[pos].x - xx.x) / mt * step;
        xx.y += (p[pos].y - xx.y) / mt * step;
        xx.z += (p[pos].z - xx.z) / mt * step;
        step *= 0.98;
    }
    return ans;
}

int main() {
    cin >> n;
    for (int i = 1; i <= n; ++i)
        cin >> p[i].x >> p[i].y >> p[i].z;
    printf("%.8lf\n", solve());
}
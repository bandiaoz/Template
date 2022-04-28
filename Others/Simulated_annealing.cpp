#include <bits/stdc++.h>

using namespace std;
using ll = long long;

const double eps = 1e-8;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;

    vector<tuple<int, int, int>> a(n);
    for (int i = 0; i < n; ++i) {
        int x, y, z;
        cin >> x >> y >> z;
        a[i] = tuple(x, y, z);
    }

    auto solve = [&]() {
        double step = 10000, ans = 1e30;
        tuple<double, double, double> tp;
        int pos = 0;

        auto dis = [&](auto A, auto B) {
            auto [x1, y1, z1] = A;
            auto [x2, y2, z2] = B;
            return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
        };

        while (step > eps) {
            for (int i = 0; i < n; ++i) { //找一个最远的点
                if (dis(tp, a[pos]) < dis(tp, a[i])) {
                    pos = i;
                }
            }  
            double mt = dis(tp, a[pos]);
            ans = min(ans, mt);
            auto [x, y, z] = tp;
            auto [px, py, pz] = a[pos];
            x += (px - x) / mt * step;
            y += (py - y) / mt * step;
            z += (pz - z) / mt * step;
            tp = tuple(x, y, z);

            step *= 0.98;
        }
        return ans;
    };

    cout << fixed << setprecision(8) << solve() << "\n";

    return 0;
}
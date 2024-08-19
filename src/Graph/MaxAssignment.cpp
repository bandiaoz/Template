#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template<class T>
struct MaxAssignment {
public:
    T solve(int nx, int ny, vector<vector<T>> a) {
        assert(0 <= nx && nx <= ny);
        assert(int(a.size()) == nx);
        for (int i = 0; i < nx; ++i) {
            assert(int(a[i].size()) == ny);
            for (auto x : a[i])
                assert(x >= 0);
        }
        
        auto update = [&](int x) {
            for (int y = 0; y < ny; ++y) {
                if (lx[x] + ly[y] - a[x][y] < slack[y]) {
                    slack[y] = lx[x] + ly[y] - a[x][y];
                    slackx[y] = x;
                }
            }
        };
        
        costs.resize(nx + 1);
        costs[0] = 0;
        lx.assign(nx, numeric_limits<T>::max());
        ly.assign(ny, 0);
        xy.assign(nx, -1);
        yx.assign(ny, -1);
        slackx.resize(ny);
        for (int cur = 0; cur < nx; ++cur) {
            queue<int> que;
            visx.assign(nx, false);
            visy.assign(ny, false);
            slack.assign(ny, numeric_limits<T>::max());
            p.assign(nx, -1);
            
            for (int x = 0; x < nx; ++x) {
                if (xy[x] == -1) {
                    que.push(x);
                    visx[x] = true;
                    update(x);
                }
            }
            
            int ex, ey;
            bool found = false;
            while (!found) {
                while (!que.empty() && !found) {
                    auto x = que.front();
                    que.pop();
                    for (int y = 0; y < ny; ++y) {
                        if (a[x][y] == lx[x] + ly[y] && !visy[y]) {
                            if (yx[y] == -1) {
                                ex = x;
                                ey = y;
                                found = true;
                                break;
                            }
                            que.push(yx[y]);
                            p[yx[y]] = x;
                            visy[y] = visx[yx[y]] = true;
                            update(yx[y]);
                        }
                    }
                }
                if (found)
                    break;
                
                T delta = numeric_limits<T>::max();
                for (int y = 0; y < ny; ++y)
                    if (!visy[y])
                        delta = min(delta, slack[y]);
                for (int x = 0; x < nx; ++x)
                    if (visx[x])
                        lx[x] -= delta;
                for (int y = 0; y < ny; ++y) {
                    if (visy[y]) {
                        ly[y] += delta;
                    } else {
                        slack[y] -= delta;
                    }
                }
                for (int y = 0; y < ny; ++y) {
                    if (!visy[y] && slack[y] == 0) {
                        if (yx[y] == -1) {
                            ex = slackx[y];
                            ey = y;
                            found = true;
                            break;
                        }
                        que.push(yx[y]);
                        p[yx[y]] = slackx[y];
                        visy[y] = visx[yx[y]] = true;
                        update(yx[y]);
                    }
                }
            }
            
            costs[cur + 1] = costs[cur];
            for (int x = ex, y = ey, ty; x != -1; x = p[x], y = ty) {
                costs[cur + 1] += a[x][y];
                if (xy[x] != -1)
                    costs[cur + 1] -= a[x][xy[x]];
                ty = xy[x];
                xy[x] = y;
                yx[y] = x;
            }
        }
        return costs[nx];
    }
    vector<int> assignment() {
        return xy;
    }
    pair<vector<T>, vector<T>> labels() {
        return make_pair(lx, ly);
    }
    vector<T> weights() {
        return costs;
    }
private:
    vector<T> lx, ly, slack, costs;
    vector<int> xy, yx, p, slackx;
    vector<bool> visx, visy;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int nx, ny;
    cin >> nx >> ny;
    ny = max(nx, ny);
    
    MaxAssignment<ll> ma;
    vector a(nx, vector<ll>(ny));
    
    int m;
    cin >> m;
    while (m--) {
        int x, y, w;
        cin >> x >> y >> w;
        x--, y--;
        a[x][y] = w;
    }
    
    cout << ma.solve(nx, ny, a) << "\n";
    auto ans = ma.assignment();
    for (int i = 0; i < nx; ++i) {
        cout << (a[i][ans[i]] == 0 ? 0 : ans[i] + 1) << " \n"[i == nx - 1];
    }
    
    return 0;
}
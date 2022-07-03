#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template<typename T>
struct KDTree {
    KDTree(int n) : n(n), lc(n, -1), rc(n, -1), L(n), R(n), U(n), D(n) {}
    KDTree(vector<array<T, 2>> &st) : KDTree(st.size()) {
        a = st;
        function<int(int, int)> innerBuild = [&](int l, int r) {
            if (l >= r) {
                return -1;
            }
            int mid = (l + r) >> 1;
            array<double, 2> average, weight;
            for (int i = l; i < r; ++i) {
                average[0] += a[i][0];
                average[1] += a[i][1];
            }
            average[0] /= r - l;
            average[1] /= r - l;
            for (int i = l; i < r; ++i) {
                weight[0] += sqr(a[i][0] - average[0]);
                weight[1] += sqr(a[i][1] - average[1]);
            }
            nth_element(a.begin() + l, a.begin() + mid, a.begin() + r, 
                [&](const auto &A, const auto &B) {
                return weight[0] > weight[1] ? A[0] < B[0] : A[1] < B[1];
            });
            lc[mid] = innerBuild(l, mid);
            rc[mid] = innerBuild(mid + 1, r);
            maintain(mid);
            return mid;
        };
        
        innerBuild(0, n);
    };
    void query(int p, double &ans) {
        innerQuery(0, n, p, ans);
    }
private:
    const int n;
    vector<int> lc, rc;
    vector<T> L, R, U, D;
    vector<array<T, 2>> a;
    
    template<typename U> U sqr(U x) { return x * x; }
    T dis(int i, int j) {
        return sqr(a[i][0] - a[j][0]) + sqr(a[i][1] - a[j][1]);
    }
    void maintain(int i) {
        L[i] = R[i] = a[i][0];
        U[i] = D[i] = a[i][1];
        if (lc[i] != -1) {
            L[i] = min(L[i], L[lc[i]]), R[i] = max(R[i], R[lc[i]]);
            D[i] = min(D[i], D[lc[i]]), U[i] = max(U[i], U[lc[i]]);
        }
        if (rc[i] != -1) {
            L[i] = min(L[i], L[rc[i]]), R[i] = max(R[i], R[rc[i]]);
            D[i] = min(D[i], D[rc[i]]), U[i] = max(U[i], U[rc[i]]);
        }
    }
    T fmin(int p, int i) { // 到这一个区域最小的距离
        T ans = 0;
        if (a[p][0] < L[i]) ans += sqr(L[i] - a[p][0]);
        if (a[p][0] > R[i]) ans += sqr(a[p][0] - R[i]);
        if (a[p][1] < D[i]) ans += sqr(D[i] - a[p][1]);
        if (a[p][1] > U[i]) ans += sqr(a[p][1] - U[i]);
        return ans;
    }
    void innerQuery(int l, int r, int p, T &ans) {
        if (l >= r) return;
        int mid = (l + r) >> 1;
        if (p != mid) {
            ans = min(ans, dis(p, mid));
        }
        if (l + 1 == r) return;

        T dl = fmin(p, lc[mid]), dr = fmin(p, rc[mid]);
        if (dl < ans && dr < ans) {
            if (dl < dr) {
                innerQuery(l, mid, p, ans);
                if (dr < ans) {
                    innerQuery(mid + 1, r, p, ans);
                }
            } else {
                innerQuery(mid + 1, r, p, ans);
                if (dl < ans) {
                    innerQuery(l, mid, p, ans);
                }
            }
        } else if (dl < ans) {
            innerQuery(l, mid, p, ans);
        } else if (dr < ans) {
            innerQuery(mid + 1, r, p, ans);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;

    vector<array<double, 2>> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i][0] >> a[i][1];
    }

    KDTree<double> kdt(a);

    double ans = 2e18;
    for (int i = 0; i < n; ++i) {
        kdt.query(i, ans);
    }

    cout << fixed << setprecision(4) << sqrt(ans) << "\n";

    return 0;
}

// test problem: https://www.luogu.com.cn/problem/P1429
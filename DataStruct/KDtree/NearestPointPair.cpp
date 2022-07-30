#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template<typename T, int K = 2>
struct KDTree {
    KDTree(int n) : n(n), lc(n, -1), rc(n, -1), boundary(n, vector<vector<T>>(K, vector<T>(2))){}
    KDTree(vector<array<T, K>> &st) : KDTree(st.size()) {
        a = st;
        function<int(int, int, int)> innerBuild = [&](int l, int r, int div) {
            if (l >= r) {
                return -1;
            }
            int mid = (l + r) >> 1;
            nth_element(a.begin() + l, a.begin() + mid, a.begin() + r, Cmp(div));
            lc[mid] = innerBuild(l, mid, (div + 1) % K);
            rc[mid] = innerBuild(mid + 1, r, (div + 1) % K);
            maintain(mid);
            return mid;
        };
        
        innerBuild(0, n, 0);
    };
    void query(int p, T &ans) {
        innerQuery(0, n, p, ans);
    }
private:
    const int n;
    vector<int> lc, rc;
    vector<vector<vector<T>>> boundary;
    vector<array<T, K>> a;

    struct Cmp {
        int div;
        Cmp(const int &div) : div(div) {}
        bool operator()(const array<T, K> &A, const array<T, K> &B) {
            for (int i = 0; i < K; ++i) {
                if (A[(i + div) % K] != B[(i + div) % K]) {
                    return A[(i + div) % K] < B[(i + div) % K];
                }
            }
            return false;
        }
    };
    bool cmp(const array<T, K> &A, const array<T, K> &B, int div) {
        Cmp cp(div);
        return cp(A, B);
    }
    template<typename U> U sqr(U x) { return x * x; }
    T dis(const array<T, K> &A, const array<T, K> &B) {
        T ans = 0;
        for (int i = 0; i < K; ++i) {
            ans += sqr(A[i] - B[i]);
        }
        return ans;
    }
    void maintain(int i) {
        for (int j = 0; j < K; ++j) {
            boundary[i][j][0] = boundary[i][j][1] = a[i][j];
            if (lc[i] != -1) {
                boundary[i][j][0] = min(boundary[i][j][0], boundary[lc[i]][j][0]);
                boundary[i][j][1] = max(boundary[i][j][1], boundary[lc[i]][j][1]);
            }
            if (rc[i] != -1) {
                boundary[i][j][0] = min(boundary[i][j][0], boundary[rc[i]][j][0]);
                boundary[i][j][1] = max(boundary[i][j][1], boundary[rc[i]][j][1]);
            }
        }
    }
    T fmin(int p, int i) { // the minimum distance to this area
        // if i == -1, ignore this area when calculating the answer.
        if (i == -1) { 
            return 1e18; 
        }
        T ans = 0;
        for (int j = 0; j < K; ++j) {
            if (a[p][j] < boundary[i][j][0]) ans += sqr(boundary[i][j][0] - a[p][j]);
            if (a[p][j] > boundary[i][j][1]) ans += sqr(a[p][j] - boundary[i][j][1]);
        }
        return ans;
    }
    void innerQuery(int l, int r, int p, T &ans) {
        if (l >= r) return;
        int mid = (l + r) >> 1;
        if (p != mid) {
            ans = min(ans, dis(a[p], a[mid]));
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
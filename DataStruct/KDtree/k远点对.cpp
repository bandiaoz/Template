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
    T query(int k) {
        priority_queue<T, vector<T>, greater<T>> q;
        for (int i = 0; i < k; ++i) q.push(0);
        for (int i = 0; i < n; ++i) {
            innerQuery(0, n, i, q);
        }
        return q.top();
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
    T fmax(int p, int i) { // 到这一个区域最大的距离
        return max(sqr(a[p][0] - L[i]), sqr(a[p][0] - R[i])) + 
               max(sqr(a[p][1] - D[i]), sqr(a[p][1] - U[i]));
    }
    void innerQuery(int l, int r, int p, priority_queue<T, vector<T>, greater<T>> &q) {
        if (l >= r) return;
        int mid = (l + r) >> 1;
        T tmp = dis(p, mid);
        if (tmp > q.top()) {
            q.pop();
            q.push(tmp);
        }
        T dl = fmax(p, lc[mid]), dr = fmax(p, rc[mid]);
        if (dl > q.top() && dr > q.top()) {
            if (dl > dr) {
                innerQuery(l, mid, p, q);
                if (dr > q.top()) {
                    innerQuery(mid + 1, r, p, q);
                }
            } else {
                innerQuery(mid + 1, r, p, q);
                if (dl > q.top()) {
                    innerQuery(l, mid, p, q);
                }
            }
        } else if (dl > q.top()) {
            innerQuery(l, mid, p, q);
        } else if (dr > q.top()) {
            innerQuery(mid + 1, r, p, q);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, k;
    cin >> n >> k;

    k *= 2;

    vector<array<ll, 2>> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i][0] >> a[i][1];
    }

    KDTree<ll> kdt(a);

    cout << kdt.query(k) << "\n";

    return 0;
}

// test problem: https://www.luogu.com.cn/problem/P4357
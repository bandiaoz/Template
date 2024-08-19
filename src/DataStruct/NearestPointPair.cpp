#include <bits/stdc++.h>

/**
 * @brief 最近点对
 * @link test problem: https://www.luogu.com.cn/problem/P1429
 * @tparam T the type of x, y
 * @tparam K 默认为 2，表示点的维度
 */
template<typename T, int K = 2>
struct KDTree {
    KDTree(int n) : n(n), lc(n, -1), rc(n, -1), boundary(n, std::vector(K, std::vector<T>(2))) {}
    KDTree(std::vector<std::array<T, K>> &st) : KDTree(st.size()) {
        a = st;
        build(0, n, 0);
    };
    void query(int p, T &ans) {
        innerQuery(0, n, p, ans);
    }

private:
    const int n;
    std::vector<int> lc, rc;
    std::vector<std::vector<std::vector<T>>> boundary;
    std::vector<std::array<T, K>> a;

    struct Cmp {
        int div;
        Cmp(const int &div) : div(div) {}
        bool operator()(const std::array<T, K> &A, const std::array<T, K> &B) {
            for (int i = 0; i < K; i++) {
                if (A[(i + div) % K] != B[(i + div) % K]) {
                    return A[(i + div) % K] < B[(i + div) % K];
                }
            }
            return false;
        }
    };
    bool cmp(const std::array<T, K> &A, const std::array<T, K> &B, int div) {
        Cmp cp(div);
        return cp(A, B);
    }
    template<typename U> U sqr(U x) { return x * x; }
    T dis(const std::array<T, K> &A, const std::array<T, K> &B) {
        T ans = 0;
        for (int i = 0; i < K; i++) {
            ans += sqr(A[i] - B[i]);
        }
        return ans;
    }

    void maintain(int i) {
        for (int j = 0; j < K; j++) {
            boundary[i][j][0] = boundary[i][j][1] = a[i][j];
            if (lc[i] != -1) {
                boundary[i][j][0] = std::min(boundary[i][j][0], boundary[lc[i]][j][0]);
                boundary[i][j][1] = std::max(boundary[i][j][1], boundary[lc[i]][j][1]);
            }
            if (rc[i] != -1) {
                boundary[i][j][0] = std::min(boundary[i][j][0], boundary[rc[i]][j][0]);
                boundary[i][j][1] = std::max(boundary[i][j][1], boundary[rc[i]][j][1]);
            }
        }
    }
    int build(int l, int r, int div) {
        if (l >= r) {
            return -1;
        }
        int mid = (l + r) >> 1;
        std::nth_element(a.begin() + l, a.begin() + mid, a.begin() + r, Cmp(div));
        lc[mid] = self(self, l, mid, (div + 1) % K);
        rc[mid] = self(self, mid + 1, r, (div + 1) % K);
        maintain(mid);
        return mid;
    }
    T fmin(int p, int i) { // the minimum distance to this area
        // if i == -1, ignore this area when calculating the answer.
        if (i == -1) { 
            return 1e18; 
        }
        T ans = 0;
        for (int j = 0; j < K; j++) {
            if (a[p][j] < boundary[i][j][0]) ans += sqr(boundary[i][j][0] - a[p][j]);
            if (a[p][j] > boundary[i][j][1]) ans += sqr(a[p][j] - boundary[i][j][1]);
        }
        return ans;
    }
    void innerQuery(int l, int r, int p, T &ans) {
        if (l >= r) return;
        int mid = (l + r) >> 1;
        if (p != mid) {
            ans = std::min(ans, dis(a[p], a[mid]));
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
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<std::array<double, 2>> a(n);
    for (int i = 0; i < n; i++) {
        std::cin >> a[i][0] >> a[i][1];
    }

    KDTree<double> kdt(a);

    double ans = 2e18;
    for (int i = 0; i < n; i++) {
        kdt.query(i, ans);
    }

    std::cout << std::fixed << std::setprecision(4) << std::sqrt(ans) << "\n";

    return 0;
}
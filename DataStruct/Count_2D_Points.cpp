#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template <typename T>
struct Fenwick {
    int n;
    std::vector<T> a;
    
    Fenwick(int n = 0) {
        init(n);
    }
    void init(int n) {
        this->n = n;
        a.assign(n, T());
    }
    void add(int x, T v) {
        for (int i = x + 1; i <= n; i += i & -i) {
            a[i - 1] += v;
        }
    }
    // return the sum of [0, x)
    T sum(int x) {
        auto ans = T();
        for (int i = x; i > 0; i -= i & -i) {
            ans += a[i - 1];
        }
        return ans;
    }
    // return the sum of [l, r)
    T rangeSum(int l, int r) {
        return sum(r) - sum(l);
    }
};

template <typename T>
struct Count_2D_Points {
    std::vector<std::pair<T, T>> a;
    std::vector<T> bx, by;
    std::vector<std::array<T, 4>> q;
    std::vector<int> ans;

    Count_2D_Points() {}
    void addPoint(T x, T y) {
        a.emplace_back(x, y);
        bx.push_back(x);
        by.push_back(y);
    }
    void addQuery(T x1, T y1, T x2, T y2, int id) {
        bx.push_back(x1 - 1);
        bx.push_back(x2);
        by.push_back(y1 - 1);
        by.push_back(y2);
        q.push_back({x2, y2, id, 1});
        q.push_back({x2, y1 - 1, id, -1});
        q.push_back({x1 - 1, y2, id, -1});
        q.push_back({x1 - 1, y1 - 1, id, 1});
    }
    std::vector<int> work() {
        sort(bx.begin(), bx.end());
        sort(by.begin(), by.end());
        bx.erase(unique(bx.begin(), bx.end()), bx.end());
        by.erase(unique(by.begin(), by.end()), by.end());
        for (int i = 0; i < a.size(); ++i) {
            a[i].first = lower_bound(bx.begin(), bx.end(), a[i].first) - bx.begin();
            a[i].second = lower_bound(by.begin(), by.end(), a[i].second) - by.begin();
        }
        for (int i = 0; i < q.size(); ++i) {
            q[i][0] = lower_bound(bx.begin(), bx.end(), q[i][0]) - bx.begin();
            q[i][1] = lower_bound(by.begin(), by.end(), q[i][1]) - by.begin();
        }

        sort(a.begin(), a.end());
        sort(q.begin(), q.end());
        Fenwick<int> fen(by.size());
        ans.resize(q.size());
        for (int i = 0, j = 0; i < q.size(); ++i) {
            while (j < a.size() && a[j].first <= q[i][0]) {
                fen.add(a[j].second, 1);
                j++;
            }
            ans[q[i][2]] += q[i][3] * fen.sum(q[i][1] + 1);
        }
        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    Count_2D_Points<int> count;
    for (int i = 0; i < n; ++i) {
        int x, y;
        cin >> x >> y;
        count.addPoint(x, y);
    }

    for (int i = 0; i < m; ++i) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        count.addQuery(x1, y1, x2, y2, i);
    }

    auto ans = count.work();
    for (int i = 0; i < m; ++i) {
        cout << ans[i] << '\n';
    }
    
    return 0;
}
// test problem: https://www.luogu.com.cn/problem/P2163
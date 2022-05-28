#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct LazySegmentTree {
    LazySegmentTree(int n) : n(n) {
        info.resize(4 * n, 0);
        tag.resize(4 * n, 0);
    }

    LazySegmentTree(vector<ll> &init) : LazySegmentTree(init.size()) {
        function<void(int, int, int)> innerBuild = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init[l];
                return;
            }
            int m = (l + r) / 2, lchild = 2 * p, rchild = 2 * p + 1;
            innerBuild(lchild, l, m);
            innerBuild(rchild, m, r);
            info[p] = info[lchild] + info[rchild];
        };
        innerBuild(1, 0, n);
    }

    /* Add val to each element in range of [x, y) */
    void update(int x, int y, ll val) {
        innerUpdate(1, x, y, val, 0, n);
    }

    /* Query the sum-up value of range [x, y) */
    ll query(int x, int y) {
        return innerQuery(1, x, y, 0, n); 
    }

private:
    /* [x, y) and val: Add val to each element in range of [x, y)
     * p: The id of subtree, which is an index of vector 'info'.
     * [l, r): The range of p.
     */
    void innerPull(int p) {
        info[p] = info[p * 2] + info[p * 2 + 1];
    }
    void innerUpdate(int p, int x, int y, ll val, int l, int r) {
        if (x <= l && r <= y) {
            info[p] += (r - l) * val;
            tag[p] += val;
            return;
        }
        int m = (l + r) / 2;
        int lchild = 2 * p, rchild = 2 * p + 1;

        if (tag[p]) {
            info[lchild] += tag[p] * (m - l), tag[lchild] += tag[p];
            info[rchild] += tag[p] * (r - m), tag[rchild] += tag[p];
            tag[p] = 0;
        }
        if (x < m) innerUpdate(lchild, x, y, val, l, m);
        if (y > m) innerUpdate(rchild, x, y, val, m, r);
        innerPull(p);
    }
    /* Query the sum-up value of range [x, y). */
    ll innerQuery(int p, int x, int y, int l, int r) {
        if (x <= l && r <= y) return info[p];
        if (x >= r || y <= l) return 0;

        int m = (l + r) / 2;
        int lchild = 2 * p, rchild = 2 * p + 1;

        if (tag[p]) {
            info[lchild] += (m - l) * tag[p], tag[lchild] += tag[p];
            info[rchild] += (r - m) * tag[p], tag[rchild] += tag[p];
            tag[p] = 0;
        }

        return innerQuery(2 * p, x, y, l, m) + innerQuery(2 * p + 1, x, y, m, r);
    }

   public:
    int n;
    vector<ll> info;  // data of segment tree, 0-index
    vector<ll> tag;  // lazy tag of segment tree
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<ll> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    LazySegmentTree seg(a);
    for (int i = 0; i < m; ++i) {
        ll op, x, y, k;
        cin >> op >> x >> y;
        x--;
        if (op == 1) {
            cin >> k;
            seg.update(x, y, k);
        } else if (op == 2) {
            cout << seg.query(x, y) << "\n";
        }
    }

    return 0;
}
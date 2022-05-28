#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct Info {
    ll c;
    Info(ll c = 0) : c(c) {}
    friend Info operator+(const Info &A, const Info &B) {
        return Info(A.c + B.c);
    }
};

void apply(Info &a, ll b, int l, int r) {
    a.c += b * (r - l);
}

void apply(ll &a, ll b, int l, int r) {
    a += b;
}

template<class Info, class Tag, class Merge = plus<Info>>
struct LazySegmentTree {
    LazySegmentTree(int n) : n(n) {
        info.resize(4 << (32 - __builtin_clz(n)), 0);
        tag.resize(4 << (32 - __builtin_clz(n)), 0);
    }

    LazySegmentTree(vector<Info> &init) : LazySegmentTree(init.size()) {
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
    void update(int x, int y, Tag v) {
        innerUpdate(1, x, y, v, 0, n);
    }

    /* Query the sum-up value of range [x, y) */
    Info query(int x, int y) {
        return innerQuery(1, x, y, 0, n); 
    }

private:
    /* [x, y) and val: Add val to each element in range of [x, y)
     * p: The id of subtree, which is an index of vector 'info'.
     * [l, r): The range of p.
     */
    void innerPull(int p) {
        info[p] = merge(info[2 * p], info[2 * p + 1]);
    }
    void innerApply(int p, const Tag &v, int l, int r) {
        ::apply(info[p], v, l, r);
        ::apply(tag[p], v, l, r);
    }
    void push(int p, int l, int r) {
        int m = (l + r) / 2;
        innerApply(2 * p, tag[p], l, m);
        innerApply(2 * p + 1, tag[p], m, r);
        tag[p] = Tag();
    }
    void innerUpdate(int p, int x, int y, const Tag &v, int l, int r) {
        if (x <= l && r <= y) {
            innerApply(p, v, l, r);
            return;
        }
        int m = (l + r) / 2;
        int lchild = 2 * p, rchild = 2 * p + 1;

        push(p, l, r);
        if (x < m) innerUpdate(lchild, x, y, v, l, m);
        if (y > m) innerUpdate(rchild, x, y, v, m, r);
        innerPull(p);
    }
    /* Query the sum-up value of range [x, y). */
    Info innerQuery(int p, int x, int y, int l, int r) {
        if (x <= l && r <= y) return info[p];
        if (x >= r || y <= l) return Info();
        int m = (l + r) / 2;
        
        push(p, l, r);
        return innerQuery(2 * p, x, y, l, m) + innerQuery(2 * p + 1, x, y, m, r);
    }

   public:
    const int n;
    const Merge merge{};
    vector<Info> info;  // data of segment tree, 0-index
    vector<Tag> tag;  // lazy tag of segment tree
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Info> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i].c;
    }

    LazySegmentTree<Info, ll> seg(a);
    for (int i = 0; i < m; ++i) {
        ll op, x, y, k;
        cin >> op >> x >> y;
        x--;
        if (op == 1) {
            cin >> k;
            seg.update(x, y, k);
        } else if (op == 2) {
            cout << seg.query(x, y).c << "\n";
        }
    }

    return 0;
}
// test problem: https://www.luogu.com.cn/problem/P3372
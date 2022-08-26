#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct Info {
    ll val;
    Info(ll val = 0) : val(val) {}
    friend Info operator+(const Info &A, const Info &B) {
        return Info(A.val + B.val);
    }
};

void apply(Info &a, ll b, int l, int r) {
    a.val += b * (r - l);
}

void apply(ll &a, ll b, int l, int r) {
    a += b;
}

template<class Info, class Tag, class Merge = plus<Info>>
class LazySegmentTree {
private:
    const int n;
    const Merge merge{};
    vector<Info> info;  // data of segment tree, 1-index
    vector<Tag> tag;  // lazy tag of segment tree

    /* [x, y) and val: Add val to each element in range of [x, y)
     * p: The id of subtree, which is an index of vector 'info'.
     * [l, r): The range of p.
     */
    void innerPull(int p) {
        info[p] = merge(info[p << 1], info[p << 1 | 1]);
    }
    void innerApply(int p, const Tag &v, int l, int r) {
        ::apply(info[p], v, l, r);
        ::apply(tag[p], v, l, r);
    }
    void push(int p, int l, int r) {
        if (tag[p] != Tag()) {
            int m = (l + r) / 2;
            innerApply(p << 1, tag[p], l, m);
            innerApply(p << 1 | 1, tag[p], m, r);
            tag[p] = Tag();
        }
    }
    void innerUpdate(int p, int x, int y, const Tag &v, int l, int r) {
        if (x <= l && r <= y) {
            innerApply(p, v, l, r);
            return;
        }
        int m = (l + r) / 2;

        push(p, l, r);
        if (x < m) innerUpdate(p << 1, x, y, v, l, m);
        if (y > m) innerUpdate(p << 1 | 1, x, y, v, m, r);
        innerPull(p);
    }
    /* Query the sum-up value of range [x, y). */
    Info innerQuery(int p, int x, int y, int l, int r) {
        if (x <= l && r <= y) return info[p];
        if (x >= r || y <= l) return Info();
        int m = (l + r) / 2;
        
        push(p, l, r);
        return merge(innerQuery(p << 1, x, y, l, m), innerQuery(p << 1 | 1, x, y, m, r));
    }

public:
    LazySegmentTree(int n) : n(n), info(4 << (32 - __builtin_clz(n))), tag(4 << (32 - __builtin_clz(n))) {}
    LazySegmentTree(vector<Info> &init) : LazySegmentTree(init.size()) {
        function<void(int, int, int)> innerBuild = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init[l];
                return;
            }
            int m = (l + r) / 2;
            innerBuild(p << 1, l, m);
            innerBuild(p << 1 | 1, m, r);
            innerPull(p);
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
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Info> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i].val;
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
            cout << seg.query(x, y).val << "\n";
        }
    }

    return 0;
}
// test problem: https://www.luogu.com.cn/problem/P3372
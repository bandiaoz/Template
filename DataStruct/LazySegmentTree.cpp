#include <bits/stdc++.h>

struct Info {
    int64_t val;
    constexpr Info(int64_t val = 0) : val(val) {}
    friend Info operator+(const Info &A, const Info &B) {
        return Info(A.val + B.val);
    }
};

template<class Info, class Tag, class Merge = std::plus<Info>>
class LazySegmentTree {
private:
    const int n;
    const Merge merge{};
    std::vector<Info> info;  // data of segment tree, 1-index
    std::vector<Tag> tag;  // lazy tag of segment tree

    /* [x, y) and val: Add val to each element in range of [x, y)
     * p: The id of subtree, which is an index of vector 'info'.
     * [l, r): The range of p.
     */
    void innerPull(int p) {
        info[p] = merge(info[p << 1], info[p << 1 | 1]);
    }
    void innerApply(int p, const Tag &v, int l, int r) {
        // TODO: update the apply function
        auto applyInfo = [&](Info &a, const Tag &b, int l, int r) {
            a.val += b * (r - l);
        };
        auto applyTag = [&](Tag &a, const Tag &b, int l, int r) {
            a += b;
        };
        applyInfo(info[p], v, l, r);
        applyTag(tag[p], v, l, r);
    }
    void push(int p, int l, int r) {
        if (tag[p] != Tag()) {
            int m = (l + r) / 2;
            innerApply(p << 1, tag[p], l, m);
            innerApply(p << 1 | 1, tag[p], m, r);
            tag[p] = Tag();
        }
    }
    void update_(int p, int x, int y, const Tag &v, int l, int r) {
        if (x <= l && r <= y) {
            innerApply(p, v, l, r);
            return;
        }
        int m = (l + r) / 2;
        push(p, l, r);
        if (x < m) update_(p << 1, x, y, v, l, m);
        if (y > m) update_(p << 1 | 1, x, y, v, m, r);
        innerPull(p);
    }
    /* Query the sum-up value of range [x, y). */
    Info query_(int p, int x, int y, int l, int r) {
        if (x <= l && r <= y) return info[p];
        if (x >= r || y <= l) return Info();
        int m = (l + r) / 2;
        push(p, l, r);
        return merge(query_(p << 1, x, y, l, m), query_(p << 1 | 1, x, y, m, r));
    }

public:
    LazySegmentTree(int n) : n(n), info(4 << std::__lg(n)), tag(4 << std::__lg(n)) {}
    LazySegmentTree(std::vector<Info> &init) : LazySegmentTree(init.size()) {
        std::function<void(int, int, int)> build_ = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init[l];
                return;
            }
            int m = (l + r) / 2;
            build_(p << 1, l, m);
            build_(p << 1 | 1, m, r);
            innerPull(p);
        };
        build_(1, 0, n);
    }
    /* Add val to each element in range of [x, y) */
    void update(int x, int y, Tag v) {
        update_(1, x, y, v, 0, n);
    }
    /* Query the sum-up value of range [x, y) */
    Info query(int x, int y) {
        return query_(1, x, y, 0, n); 
    }
};

using namespace std;
using ll = long long;

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
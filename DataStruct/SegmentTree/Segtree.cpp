#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template<class Info, class Merge = plus<Info>>
struct SegmentTree {
    const int n;
    const Merge merge;
    vector<Info> info;
    SegmentTree(int n) : n(n), merge(Merge()), info(4 << (32 - __builtin_clz(n))) {}
    SegmentTree(vector<Info> init) : SegmentTree(init.size()) {
        function<void(int, int, int)> build = [&](int root, int l, int r) {
            if (r - l == 1) {
                info[root] = init[l];
                return;
            }
            int mid = (l + r) / 2;
            build(root << 1, l, mid);
            build(root << 1 | 1, mid, r);
            pull(root);
        };
        build(1, 0, n);
    }
    void pull(int root) {
        info[root] = merge(info[root << 1], info[root << 1 | 1]);
    }
    void modify(int root, int l, int r, int pos, const Info &x) {
        if (r - l == 1) {
            info[root] = info[root] + x;
            return;
        }
        int mid = (l + r) / 2;
        if (pos < mid) {
            modify(root << 1, l, mid, pos, x);
        } else {
            modify(root << 1 | 1, mid, r, pos, x);
        }
        pull(root);
    }
    void modify(int pos, const Info &x) {
        modify(1, 0, n, pos, x);
    }
    Info rangeQuery(int root, int l, int r, int x, int y) {
        if (l >= y || r <= x) return Info();
        if (l >= x && r <= y) return info[root];
        int mid = (l + r) / 2;
        return merge(rangeQuery(root << 1, l, mid, x, y), rangeQuery(root << 1 | 1, mid, r, x, y));
    }
    Info rangeQuery(int l, int r) {
        return rangeQuery(1, 0, n, l, r);
    }
};

struct info {
    int val;
    info(int val = 0) : val(val) {}
    friend info operator+(const info &A, const info &B) {
        return info(A.val + B.val);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    SegmentTree<info> seg(n);
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        seg.modify(i, x);
    }

    while (m--) {
        int op, x, y;
        cin >> op;
        if (op == 1) {
            cin >> x >> y;
            x--;
            seg.modify(x, y);
        } else {
            cin >> x >> y;
            x--;
            cout << seg.rangeQuery(x, y).val << "\n";
        }
    }

    return 0;
}

// test problem: https://www.luogu.com.cn/problem/P3374
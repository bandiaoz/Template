#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template<class Info, class Merge = plus<Info>>
struct SegmentTree {
    SegmentTree(int n) : n(n), merge(Merge()), info(4 << (32 - __builtin_clz(n))) {}
    SegmentTree(vector<Info> init) : SegmentTree(init.size()) {
        function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init[l];
                return;
            }
            int mid = (l + r) / 2;
            build(p << 1, l, mid);
            build(p << 1 | 1, mid, r);
            innerPull(p);
        };
        build(1, 0, n);
    }
    void modify(int pos, const Info &x) {
        innerModify(1, 0, n, pos, x);
    }
    Info rangeQuery(int l, int r) {
        return innerRangeQuery(1, 0, n, l, r);
    }

private:
    const int n;
    const Merge merge;
    vector<Info> info;
    void innerPull(int p) {
        info[p] = merge(info[p << 1], info[p << 1 | 1]);
    }
    void innerModify(int p, int l, int r, int pos, const Info &x) {
        if (r - l == 1) {
            info[p] = info[p] + x;
            return;
        }
        int mid = (l + r) / 2;
        if (pos < mid) {
            innerModify(p << 1, l, mid, pos, x);
        } else {
            innerModify(p << 1 | 1, mid, r, pos, x);
        }
        innerPull(p);
    }
    Info innerRangeQuery(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) return Info();
        if (l >= x && r <= y) return info[p];
        int mid = (l + r) / 2;
        return merge(innerRangeQuery(p << 1, l, mid, x, y), innerRangeQuery(p << 1 | 1, mid, r, x, y));
    }
};

struct Info {
    int val;
    Info(int val = 0) : val(val) {}
    friend Info operator+(const Info &A, const Info &B) {
        return Info(A.val + B.val);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    SegmentTree<Info> seg(n);
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
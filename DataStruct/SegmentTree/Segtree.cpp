#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template<class Info, class Merge = plus<Info>>
struct SegmentTree {
    SegmentTree(int n) : n(n), merge(Merge()), info(4 << (32 - __builtin_clz(n))) {}
    SegmentTree(vector<Info> init) : SegmentTree(init.size()) {
        function<void(int, int, int)> _build = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init[l];
                return;
            }
            int mid = (l + r) / 2;
            _build(p << 1, l, mid);
            _build(p << 1 | 1, mid, r);
            _pull(p);
        };
        _build(1, 0, n);
    }
    void modify(int pos, const Info &x) {
        _modify(1, 0, n, pos, x);
    }
    Info rangeQuery(int l, int r) {
        return _rangeQuery(1, 0, n, l, r);
    }

private:
    const int n;
    const Merge merge;
    vector<Info> info;
    void _pull(int p) {
        info[p] = merge(info[p << 1], info[p << 1 | 1]);
    }
    void _modify(int p, int l, int r, int pos, const Info &x) {
        if (r - l == 1) {
            info[p] = info[p] + x;
            return;
        }
        int mid = (l + r) / 2;
        if (pos < mid) {
            _modify(p << 1, l, mid, pos, x);
        } else {
            _modify(p << 1 | 1, mid, r, pos, x);
        }
        _pull(p);
    }
    Info _rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) return Info();
        if (l >= x && r <= y) return info[p];
        int mid = (l + r) / 2;
        return merge(_rangeQuery(p << 1, l, mid, x, y), _rangeQuery(p << 1 | 1, mid, r, x, y));
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
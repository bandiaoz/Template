// reference: https://atcoder.github.io/ac-library/master/document_en/segtree.html

#include <bits/stdc++.h>

using namespace std;
using ll = long long;

constexpr unsigned ceil_lg(int n) {
    return n == 0 ? 0 : 32 - __builtin_clz(n - 1);
}
template <typename T> struct Segtree {
  public:
    Segtree() : Segtree(0) {}
    explicit Segtree(int n) : Segtree(vector<typename T::S>(n, T::e())) {}
    explicit Segtree(const vector<typename T::S>& a) : _n(int(a.size())) {
        log = ceil_lg(_n);
        size = 1 << log;
        d = vector<typename T::S>(2 * size, T::e());
        for (int i = 0; i < _n; i++) d[size + i] = a[i];
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }
    void set(int p, typename T::S x) {
        assert(0 <= p && p < _n);
        p += size;
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    typename T::S get(int p) const {
        assert(0 <= p && p < _n);
        return d[p + size];
    }
    typename T::S query(int l, int r) const {
        assert(0 <= l && l <= r && r <= _n);
        typename T::S sml = T::e(), smr = T::e();
        l += size;
        r += size;
        while (l < r) {
            if (l & 1) sml = T::op(sml, d[l++]);
            if (r & 1) smr = T::op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return T::op(sml, smr);
    }
    typename T::S queryAll() const { return d[1]; }
    template <bool (*f)(typename T::S)> int max_right(int l) const {
        return max_right(l, [](typename T::S x) { return f(x); });
    }
    // r = l or f(op(a[l], ..., a[r - 1])) = true
    // r = n or f(op(a[l], ..., a[r])) = false
    template <class F> int max_right(int l, F f) const {
        assert(0 <= l && l <= _n);
        assert(f(T::e()));
        if (l == _n) return _n;
        l += size;
        typename T::S sm = T::e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(T::op(sm, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(T::op(sm, d[l]))) {
                        sm = T::op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = T::op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }
    template <bool (*f)(typename T::S)> int min_left(int r) const {
        return min_left(r, [](typename T::S x) { return f(x); });
    }
    // r = l or f(op(a[l], ..., a[r - 1])) = true
    // r = n or f(op(a[l - 1], ..., a[r - 1])) = false
    template <class F> int min_left(int r, F f) const {
        assert(0 <= r && r <= _n);
        assert(f(T::e()));
        if (r == 0) return 0;
        r += size;
        typename T::S sm = T::e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(T::op(d[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(T::op(d[r], sm))) {
                        sm = T::op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = T::op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
  private:
    int _n, size, log;
    vector<typename T::S> d;
    void update(int k) { d[k] = T::op(d[2 * k], d[2 * k + 1]); }
};

struct SegtreeOP {
    using S = int;
    static S e() { return -1; }
    static S op(const S &x, const S &y) { 
        return max(x, y); 
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    Segtree<SegtreeOP> seg(a);
    for (int i = 0; i < m; ++i) {
        int op;
        cin >> op;

        if (op == 1) {
            int x, v;
            cin >> x >> v;
            x--;
            seg.set(x, v);
        } else if (op == 2) {
            int l, r;
            cin >> l >> r;
            l--;
            cout << seg.query(l, r) << "\n";
        } else {
            int x, v;
            cin >> x >> v;
            x--;
            cout << seg.max_right(x, [&](int a) { return a < v; }) + 1 << "\n";
        }
    }

    return 0;
}

// test problem: https://atcoder.jp/contests/practice2/tasks/practice2_j
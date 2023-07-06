#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template <typename T, auto f = std::multiplies()>
constexpr static T qpow(T a, int64_t b) {
    T res;
    if constexpr (std::is_arithmetic_v<T>) {
        res = 1;
    } else {
        res = a.identity();
    }
    while (b) {
        if (b & 1) {
            res = f(res, a);
        }
        b >>= 1;
        a = f(a, a);
    }
    return res;
}

template <typename T, T MOD>
struct ModInt {
    using prod_type = std::conditional_t<std::is_same_v<T, int>, long long, __int128>;
    T val;
    constexpr ModInt(const int64_t v = 0) : val(v % MOD) { if (val < 0) val += MOD; }
    constexpr ModInt operator+() const { return ModInt(val); }
    constexpr ModInt operator-() const { return ModInt(MOD - val); }
    constexpr ModInt inv() const {
        return qpow(*this, MOD - 2);
    }
    constexpr friend ModInt operator+(ModInt lhs, const ModInt rhs) { return lhs += rhs; }
    constexpr friend ModInt operator-(ModInt lhs, const ModInt rhs) { return lhs -= rhs; }
    constexpr friend ModInt operator*(ModInt lhs, const ModInt rhs) { return lhs *= rhs; }
    constexpr friend ModInt operator/(ModInt lhs, const ModInt rhs) { return lhs /= rhs; }
    constexpr ModInt &operator+=(const ModInt x) {
        if ((val += x.val) >= MOD)
            val -= MOD;
        return *this;
    }
    constexpr ModInt &operator-=(const ModInt x) {
        if ((val -= x.val) < 0)
            val += MOD;
        return *this;
    }
    constexpr ModInt &operator*=(const ModInt x) {
        val = prod_type(val) * x.val % MOD;
        return *this;
    }
    constexpr ModInt &operator/=(const ModInt x) { return *this *= x.inv(); }
    bool operator==(const ModInt b) const { return val == b.val; }
    bool operator!=(const ModInt b) const { return val != b.val; }
    friend std::istream &operator>>(std::istream &is, ModInt &x) noexcept {
        return is >> x.val;
    }
    friend std::ostream &operator<<(std::ostream &os, const ModInt x) noexcept {
        return os << x.val;
    }
    constexpr static ModInt identity() { return 1; }
    constexpr ModInt pow(int64_t p) {
        return qpow(*this, p);
    }
};
using Z = ModInt<int, 571373>;

struct Info {
    Z val;
    constexpr Info(Z val = 0) : val(val) {}
    friend Info operator+(const Info &A, const Info &B) {
        return Info(A.val + B.val);
    }
};

struct Tag {
    Z add, mul;
    constexpr Tag(Z add = 0, Z mul = 1) : add(add), mul(mul) {}
    friend bool operator==(const Tag &A, const Tag &B) {
        return A.add == B.add && A.mul == B.mul;
    }
    friend bool operator!=(const Tag &A, const Tag &B) {
        return !(A == B);
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
            a.val = a.val * b.mul + b.add * (r - l);
        };
        auto applyTag = [&](Tag &a, const Tag &b, int l, int r) {
            a.add = a.add * b.mul + b.add;
            a.mul *= b.mul;
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, p;
    cin >> n >> m >> p;
    vector<Info> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i].val;
    }

    LazySegmentTree<Info, Tag> seg(a);
    while (m--) {
        int op, l, r, x;
        cin >> op >> l >> r;
        l--;
        if (op == 1) {
            cin >> x;
            seg.update(l, r, Tag(0, x));
        } else if (op == 2) {
            cin >> x;
            seg.update(l, r, Tag(x, 1));
        } else {
            cout << seg.query(l, r).val << "\n";
        }
    }

    return 0;
}
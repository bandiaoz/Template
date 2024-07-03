#include <bits/stdc++.h>
#include "../Others/Z.hpp"

struct MOD { static int value; };
int MOD::value = 571373;
using Z = ModInt<MOD>;

/**
 * @brief Lazy Segment Tree
 * @link https://www.luogu.com.cn/problem/P3373
 * @link https://judge.yosupo.jp/problem/range_affine_range_sum
 */
template <class Info, class Tag,
          class FunInfo = std::function<void(Info&, const Tag&, const int, const int)>,
          class FunTag = std::function<void(Tag&, const Tag&, const int, const int)>,
          class Merge = std::plus<Info>>
struct LazySegmentTree {
    LazySegmentTree() : n(0) {}
    LazySegmentTree(int n, Info v, const FunInfo f1, const FunTag f2) {
        init(std::vector(n, v), f1, f2);
    }
    LazySegmentTree(int n, const FunInfo f1, const FunTag f2) {
        init(std::vector(n, Info()), f1, f2);
    }
    LazySegmentTree(std::vector<Info> &list, const FunInfo f1, const FunTag f2) {
        init(list, f1, f2);
    }

    void init(int n, Info v = Info()) { init(std::vector(n, v)); }
    template <class T>
    void init(std::vector<T> list, const FunInfo f1 = FunInfo(), const FunTag f2 = FunTag()) {
        n = list.size();
        applyInfo = f1;
        applyTag = f2;
        info.assign(4 << std::__lg(n), Info());
        tag.resize(4 << std::__lg(n), Tag());
        build(1, 0, n, list);
    }
    void update(int x, int y, Tag v) { update_(1, x, y, v, 0, n); }
    Info query(int x, int y) { return query_(1, x, y, 0, n); }

private:
    int n;
    const Merge merge{};
    FunInfo applyInfo;
    FunTag applyTag;
    std::vector<Info> info;
    std::vector<Tag> tag;

    void build(int p, int l, int r, const std::vector<Info> &list) {
        if (r - l == 1) {
            info[p] = list[l];
            return;
        }
        int mid = (l + r) / 2;
        build(p << 1, l, mid, list);
        build(p << 1 | 1, mid, r, list);
        pull(p);
    }
    void pull(int p) { 
        info[p] = merge(info[p << 1], info[p << 1 | 1]); 
    }
    void apply(int p, const Tag &v, int l, int r) {
        applyInfo(info[p], v, l, r);
        applyTag(tag[p], v, l, r);
    }
    void push(int p, int l, int r) {
        if (tag[p] != Tag()) {
            int m = (l + r) / 2;
            apply(p << 1, tag[p], l, m);
            apply(p << 1 | 1, tag[p], m, r);
            tag[p] = Tag();
        }
    }
    void update_(int p, int x, int y, const Tag &v, int l, int r) {
        if (x <= l && r <= y) {
            apply(p, v, l, r);
            return;
        }
        int m = (l + r) / 2;
        push(p, l, r);
        if (x < m) update_(p << 1, x, y, v, l, m);
        if (y > m) update_(p << 1 | 1, x, y, v, m, r);
        pull(p);
    }
    Info query_(int p, int x, int y, int l, int r) {
        if (x <= l && r <= y) return info[p];
        if (x >= r || y <= l) return Info();
        int m = (l + r) / 2;
        push(p, l, r);
        return merge(query_(p << 1, x, y, l, m), query_(p << 1 | 1, x, y, m, r));
    }
};
struct Info {
    Z val;
    constexpr Info(Z val = 0) : val(val) {}
    friend Info operator+(const Info &A, const Info &B) {
        return Info(A.val + B.val);
    }
};
struct Tag {
    Z mul, add;
    constexpr Tag(Z mul = 1, Z add = 0) : mul(mul), add(add) {}
    friend bool operator==(const Tag &A, const Tag &B) {
        return A.mul == B.mul && A.add == B.add;
    }
    friend bool operator!=(const Tag &A, const Tag &B) { return !(A == B); }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, p;
    std::cin >> n >> m >> p;
    std::vector<Info> a(n);
    for (auto &i : a) {
        std::cin >> i.val;
    }

    auto applyInfo = [&](Info &a, const Tag &b, int l, int r) {
        a.val = a.val * b.mul + b.add * (r - l);
    };
    auto applyTag = [&](Tag &a, const Tag &b, int l, int r) {
        a.add = a.add * b.mul + b.add;
        a.mul *= b.mul;
    };
    LazySegmentTree<Info, Tag> seg(a, applyInfo, applyTag);
    while (m--) {
        int op, l, r, x;
        std::cin >> op >> l >> r;
        l--;
        if (op == 1) {
            std::cin >> x;
            seg.update(l, r, Tag(x, 0));
        } else if (op == 2) {
            std::cin >> x;
            seg.update(l, r, Tag(1, x));
        } else {
            std::cout << seg.query(l, r).val << "\n";
        }
    }

    return 0;
}
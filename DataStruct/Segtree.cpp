#include <bits/stdc++.h>

/**
 * @brief Segment Tree 线段树
 * @note 需要满足结合律，即 merge(a, merge(b, c)) = merge(merge(a, b), c)
 * @note Info::identity() 需要满足 merge(info, identity()) = info
*/
template<class Info, class Merge = std::plus<Info>>
struct SegmentTree {
    SegmentTree() : n(0) {}
    SegmentTree(int n, Info v = Info()) { init(n, v); }
    template<class T>
    SegmentTree(std::vector<T> list) { init(list); }

    void init(int n, Info v = Info()) { init(std::vector(n, v)); }
    template<class T>
    void init(std::vector<T> list) {
        n = list.size();
        merge = Merge();
        info.assign(4 << std::__lg(n), Info());
        build(1, 0, n, list);
    }
    /**
     * modify(pos, x) -> info[pos] = x
     */
    void modify(int pos, const Info &x) {
        assert(0 <= pos && pos < n);
        modify(1, 0, n, pos, x, [](const Info &a, const Info &b) { return b; });
    }
    /**
     * modify(pos, x, func) -> info[pos] = func(info[pos], x)
     */
    template<class F>
    void modify(int pos, const Info &x, F func) {
        assert(0 <= pos && pos < n);
        modify(1, 0, n, pos, x, func);
    }
    /**
     * @if l < r, return merge(info[l], info[l + 1], ..., info[r - 1])
     * @else return Info::identity()
     */
    Info rangeQuery(int l, int r) {
        return rangeQuery(1, 0, n, l, r);
    }
    /**
     * @link https://atcoder.jp/contests/practice2/tasks/practice2_j
     * @return the first pos in [l, r) that pred(info[pos]) is true
     *         return -1 if not found
     */
    template<class F>
    int findFirst(int l, int r, F pred) {
        return findFirst(1, 0, n, l, r, pred);
    }
    /**
     * @return the last pos in [l, r) that pred(info[pos]) is true
     *         return -1 if not found
     */
    template<class F>
    int findLast(int l, int r, F pred) {
        return findLast(1, 0, n, l, r, pred);
    }

private:
    int n;
    Merge merge;
    std::vector<Info> info;
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
    template<class F>
    void modify(int p, int l, int r, int pos, const Info &x, const F &func) {
        if (r - l == 1) {
            info[p] = func(info[p], x);
            return;
        }
        int mid = (l + r) / 2;
        if (pos < mid) {
            modify(p << 1, l, mid, pos, x, func);
        } else {
            modify(p << 1 | 1, mid, r, pos, x, func);
        }
        pull(p);
    }
    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) return Info::identity();
        if (x <= l && r <= y) return info[p];
        int mid = (l + r) / 2;
        return merge(rangeQuery(p << 1, l, mid, x, y), rangeQuery(p << 1 | 1, mid, r, x, y));
    }
    template<class F>
    int findFirst(int p, int l, int r, int x, int y, F pred) {
        if (l >= y || r <= x || !pred(info[p])) return -1;
        if (r - l == 1) return l;
        int mid = (l + r) / 2;
        int ans = findFirst(p << 1, l, mid, x, y, pred);
        if (ans == -1) {
            ans = findFirst(p << 1 | 1, mid, r, x, y, pred);
        }
        return ans;
    }
    template<class F>
    int findLast(int p, int l, int r, int x, int y, F pred) {
        if (l >= y || r <= x || !pred(info[p])) return -1;
        if (r - l == 1) return l;
        int mid = (l + r) / 2;
        int ans = findLast(p << 1 | 1, mid, r, x, y, pred);
        if (ans == -1) {
            ans = findLast(p << 1, l, mid, x, y, pred);
        }
        return ans;
    }
};
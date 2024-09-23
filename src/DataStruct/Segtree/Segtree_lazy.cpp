#include <bits/stdc++.h>

/**
 * @brief Lazy Segment Tree
 */
template <class Info, class Tag, class Apply, class Merge = std::plus<Info>>
struct LazySegmentTree {
    LazySegmentTree() : n(0) {}
    LazySegmentTree(int n, Info v = Info()) { init(std::vector(n, v)); }
    template <class T>
    LazySegmentTree(std::vector<T> &list) { init(list); }

    void init(int n, Info v = Info()) { init(std::vector(n, v)); }
    template <class T>
    void init(std::vector<T> list) {
        n = list.size();
        info.assign(4 << std::__lg(n), Info());
        tag.resize(4 << std::__lg(n), Tag());
        build(1, 0, n, list);
    }
    /**
     * modify(x, y, v) -> info[i] = apply(info[i], v) for x <= i < y
     */
    void modify(int x, int y, Tag v) { 
        assert(0 <= x && x <= y && y <= n);
        update(1, x, y, v, 0, n);
    }
    /**
     * @if l < r, return merge(info[l], info[l + 1], ..., info[r - 1])
     * @else return Info::identity()
     */
    Info query(int x, int y) { 
        return query(1, x, y, 0, n); 
    }

private:
    int n;
    const Merge merge{};
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
    void apply(int p, const Tag &v) {
        Apply::applyInfo(info[p], v);
        Apply::applyTag(tag[p], v);
    }
    void push(int p, int l, int r) {
        if (tag[p] != Tag::identity()) {
            int mid = (l + r) / 2;
            apply(p << 1, tag[p]);
            apply(p << 1 | 1, tag[p]);
            tag[p] = Tag::identity();
        }
    }
    void update(int p, int x, int y, const Tag &v, int l, int r) {
        if (x <= l && r <= y) {
            apply(p, v);
            return;
        }
        int mid = (l + r) / 2;
        push(p, l, r);
        if (x < mid) update(p << 1, x, y, v, l, mid);
        if (y > mid) update(p << 1 | 1, x, y, v, mid, r);
        pull(p);
    }
    Info query(int p, int x, int y, int l, int r) {
        if (x <= l && r <= y) return info[p];
        if (x >= r || y <= l) return Info::identity();
        int mid = (l + r) / 2;
        push(p, l, r);
        return merge(query(p << 1, x, y, l, mid), query(p << 1 | 1, x, y, mid, r));
    }
};
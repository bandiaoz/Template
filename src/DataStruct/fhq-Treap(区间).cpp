#include <bits/stdc++.h>

using namespace std;
using ll = long long;

mt19937 rnd(random_device{}());
class fhqtreap {
    struct node {
        int val, siz, tag;
        mt19937::result_type rnd;
        node *ch[2];
    } *root = nullptr;
    int size(node *rt) {
        return rt ? rt->siz : 0;
    };
    void maintain(node *rt) {
        rt->siz = size(rt->ch[0]) + size(rt->ch[1]) + 1;
    }
    void spread(node *rt) {
        if (!rt->tag)
            return;

        swap(rt->ch[0], rt->ch[1]);

        if (rt->ch[0])
            rt->ch[0]->tag ^= 1;

        if (rt->ch[1])
            rt->ch[1]->tag ^= 1;

        rt->tag = 0;
    }
    pair<node *, node *> split(node *rt, int x) {
        if (!rt)
            return {};

        spread(rt);

        if (size(rt->ch[0]) >= x) {
            auto [l, r] = split(rt->ch[0], x);
            rt->ch[0] = r, maintain(rt);
            return {l, rt};
        } else {
            auto [l, r] = split(rt->ch[1], x - size(rt->ch[0]) - 1);
            rt->ch[1] = l, maintain(rt);
            return {rt, r};
        }
    }
    node *merge(node *lt, node *rt) {
        if (!(lt && rt))
            return lt ? lt : rt;

        spread(lt), spread(rt);

        if (lt->rnd < rt->rnd) {
            lt->ch[1] = merge(lt->ch[1], rt), maintain(lt);
            return lt;
        } else {
            rt->ch[0] = merge(lt, rt->ch[0]), maintain(rt);
            return rt;
        }
    }
    void output(node *rt, ostream &os) {
        if (!rt)
            return;

        spread(rt);
        output(rt->ch[0], os);
        os << rt->val << ' ';
        output(rt->ch[1], os);
    }

public:
    void insert(int x) {
        // auto [p, r] = split(root, x);
        // auto [l, m] = split(p, x - 1);
        auto m = new node{x, 1, 0, rnd()};
        root = merge(root, m);
    }
    void reverse(int x, int y) {
        auto [p, r] = split(root, y);
        auto [l, m] = split(p, x);
        m->tag ^= 1;
        root = merge(merge(l, m), r);
    }
    friend ostream &operator<<(ostream &os, fhqtreap &rhs) {
        return rhs.output(rhs.root, os), os;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    fhqtreap tree;
    for (int i = 1; i <= n; ++i) {
        tree.insert(i);
    }

    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        x--;
        
        tree.reverse(x, y);
    }

    cout << tree;

    return 0;
}
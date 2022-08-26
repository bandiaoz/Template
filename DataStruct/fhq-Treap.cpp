#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template<typename key_t>
struct Treap {
    struct Node {
        key_t key;
        int pri;
        int l, r, sz;
        Node(key_t a, int b) : key(a), pri(b), l(-1), r(-1), sz(1) {}
    };

    int root = -1;
    vector<Node> tree;

    // split by key, the key of x treap less than y treap
    array<int, 2> split(int pos, key_t key) {
        if (pos == -1) return {-1, -1};

        if (tree[pos].key <= key) {
            array<int, 2> res = split(tree[pos].r, key);
            tree[pos].r = res[0];
            update(pos);
            return {pos, res[1]};
        } else {
            array<int, 2> res = split(tree[pos].l, key);
            tree[pos].l = res[1];
            update(pos);
            return {res[0], pos};
        }
    }
    // split by size, the size of x treap equal to sz
    array<int, 2> split_sz(int pos, int sz) {
        if (pos == -1) return {-1, -1};

        if (tree[tree[pos].l].sz + 1 <= sz) {
            array<int, 2> res = split_sz(tree[pos].r, sz - tree[tree[pos].l].sz - 1);
            tree[pos].r = res[0];
            update(pos);
            return {pos, res[1]};
        } else {
            array<int, 2> res = split_sz(tree[pos].l, sz);
            tree[pos].l = res[1];
            update(pos);
            return {res[0], pos};
        }
    }
    // small root heap, the key of x treap less than y treap
    int merge(int x, int y) {
        if (x == -1) return y;
        if (y == -1) return x;

        if (tree[x].pri > tree[y].pri) {
            swap(x, y);
        }

        array<int, 2> res = split(y, tree[x].key);
        tree[x].l = merge(tree[x].l, res[0]);
        tree[x].r = merge(tree[x].r, res[1]);
        update(x);
        return x;
    }
    void update(int pos) {
        tree[pos].sz = tree[tree[pos].l].sz + tree[tree[pos].r].sz + 1;
    }
    int create(key_t key) {
        mt19937 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());
        int pri = (int)(rng() & ((1ll << 31) - 1));
        tree.emplace_back(key, pri);
        return (int)tree.size() - 1;
    }
    void insert(int &pos, key_t key) {
        int o = create(key);
        array<int, 2> res = split(pos, key);
        pos = merge(merge(res[0], o), res[1]);
    }
    // Return rank with power is key
    int rank(int &pos, key_t key) {
        array<int, 2> res = split(pos, key - 1);
        int rk = (res[0] == -1) ? 1 : tree[res[0]].sz + 1;
        pos = merge(res[0], res[1]);
        return rk;
    }
    // Return the key of the k largest
    key_t kth(int &pos, int k) {
        assert(k <= tree[pos].sz);
        array<int, 2> res1 = split_sz(pos, k);
        array<int, 2> res2 = split_sz(res1[0], k - 1);
        key_t key = tree[res2[1]].key;
        pos = merge(merge(res2[0], res2[1]), res1[1]);
        return key;
    }
    // Delete one node that equal to key
    void erase(int &pos, key_t key) {
        array<int, 2> res1 = split(pos, key);
        array<int, 2> res2 = split(res1[0], key - 1);

        if (res2[1] != -1) {
            res2[1] = merge(tree[res2[1]].l, tree[res2[1]].r);
        }

        pos = merge(merge(res2[0], res2[1]), res1[1]);
    }
    // Return the precursor of key
    key_t pre(int &pos, key_t key) {
        array<int, 2> res = split(pos, key - 1);
        key_t ans = kth(res[0], tree[res[0]].sz);
        pos = merge(res[0], res[1]);
        return ans;
    }
    // Return the next of key
    key_t nxt(int &pos, key_t key) {
        array<int, 2> res = split(pos, key);
        int ans = kth(res[1], 1);
        pos = merge(res[0], res[1]);
        return ans;
    }

    void insert(key_t x) { insert(root, x); }
    void erase(int x) { erase(root, x); }
    int rank(key_t x) { return rank(root, x); }
    key_t kth(int x) { return kth(root, x); }
    key_t pre(key_t x) { return pre(root, x); }
    key_t nxt(key_t x) { return nxt(root, x); }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    Treap<int> T;

    for (int i = 1; i <= n; i++) {
        int op, x;
        cin >> op >> x;

        if (op == 1) {
            T.insert(x);
        } else if (op == 2) {
            T.erase(x);
        } else if (op == 3) {
            cout << T.rank(x) << "\n";
        } else if (op == 4) {
            cout << T.kth(x) << "\n";
        } else if (op == 5) {
            cout << T.pre(x) << "\n";
        } else if (op == 6) {
            cout << T.nxt(x) << "\n";
        }
    }

    return 0;
}

// test problem: https://loj.ac/p/104
/**
 * test: https://www.luogu.com.cn/problem/P3369
*/
#include <bits/stdc++.h>

template <typename T>
class FHQTreap {
   private:
    struct node {
        T val;
        size_t prio;
        int left, right;
        size_t size;

        node() : prio(rand()), left(0), right(0), size(1) {}
        node(T _val) : val(_val), prio(rand()), left(0), right(0), size(1) {}
        node(T _val, size_t _size)
            : val(_val), prio(0), left(0), right(0), size(_size) {}
    };
    std::vector<node> tr;
    int root;

    void create(int& k, T val) {
        tr.push_back(node(val));
        k = tr.size() - 1;
    }

    void pushup(int p) {
        tr[p].size = tr[tr[p].left].size + tr[tr[p].right].size + 1;
    }

    int merge(int x, int y) {
        if (!x || !y) return x | y;
        if (tr[x].prio < tr[y].prio) {
            tr[x].right = merge(tr[x].right, y);
            pushup(x);
            return x;
        } else {
            tr[y].left = merge(x, tr[y].left);
            pushup(y);
            return y;
        }
    }

    void split(int p, T k, int& x, int& y) {
        if (!p) {
            x = y = 0;
            return;
        }
        if (tr[p].val <= k) {
            x = p;
            split(tr[x].right, k, tr[x].right, y);
            pushup(x);
        } else {
            y = p;
            split(tr[y].left, k, x, tr[y].left);
            pushup(y);
        }
    }

    void insert(int& rt, T k) {
        int x = 0, y = 0, z = 0;
        split(rt, k, x, y);
        create(z, k);
        rt = merge(merge(x, z), y);
    }

    void remove(int& rt, T k) {
        int x = 0, y = 0, z = 0;
        split(rt, k, x, z);
        split(x, k - 1, x, y);
        y = merge(tr[y].left, tr[y].right);
        rt = merge(merge(x, y), z);
    }

    T kth(int p, int rank) {
        int ls = tr[tr[p].left].size;
        if (rank == ls + 1) return tr[p].val;
        if (rank <= ls) return kth(tr[p].left, rank);
        return kth(tr[p].right, rank - ls - 1);
    }

    int rank(int& rt, T k) {
        int x, y;
        split(rt, k - 1, x, y);
        int rank = tr[x].size + 1;
        rt = merge(x, y);
        return rank;
    }

    T lower_bound(int& rt, T v) {
        int x, y, k, pre;
        split(rt, v - 1, x, y);
        if (!x) return std::numeric_limits<T>::min();
        k = tr[x].size;
        pre = kth(x, k);
        rt = merge(x, y);
        return pre;
    }

    T upper_bound(int& rt, T v) {
        int x, y, nxt;
        split(rt, v, x, y);
        if (!y) return std::numeric_limits<T>::max();
        nxt = kth(y, 1);
        rt = merge(x, y);
        return nxt;
    }

   public:
    FHQTreap() : root(0) {
        srand(19260817);
        tr.push_back(node(0, 0));
    }

    void insert(T k) { insert(root, k); }
    void remove(T k) { remove(root, k); }
    T kth(int rank) { return kth(root, rank); }
    int rank(T k) { return rank(root, k); }
    T lower_bound(T k) { return lower_bound(root, k); }
    T upper_bound(T k) { return upper_bound(root, k); }
    size_t size() { return tr[root].size; }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    FHQTreap<int> fhq;
    while (n--) {
        int op, x;
        std::cin >> op >> x;

        if (op == 1) {
            fhq.insert(x);
        } else if (op == 2) {
            fhq.remove(x);
        } else if (op == 3) {
            std::cout << fhq.rank(x) << "\n";
        } else if (op == 4) {
            std::cout << fhq.kth(x) << "\n";
        } else if (op == 5) {
            std::cout << fhq.lower_bound(x) << "\n";
        } else if (op == 6) {
            std::cout << fhq.upper_bound(x) << "\n";
        }
    }

    return 0;
}
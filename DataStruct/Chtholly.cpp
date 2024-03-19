#include <bits/stdc++.h>

struct Chtholly {
    struct node {
        int l, r;
        mutable int64_t v;

        node(int l, int r, int64_t v) : l(l), r(r), v(v) {}
        int size() const { return r - l; }
        bool operator<(const node &A) const { return l < A.l; }
    };

    std::set<node> s;
    auto insert(int l, int r, int64_t v) { return s.insert(node(l, r, v)); }
    // 拆区间，将区间分为 [l,pos), [pos,r) 两段
    auto split(int pos) {
        auto it = s.lower_bound(node(pos, -1, 0));
        if (it != s.end() && it->l == pos) {
            return it;
        }
        --it;
        int L = it->l, R = it->r;
        int64_t V = it->v;
        s.erase(it);
        insert(L, pos, V);
        // 返回第二个区间的地址
        return insert(pos, R, V).first;
    }
    void add(int l, int r, int64_t x) {
        for (auto itr = split(r), itl = split(l); itl != itr; ++itl) {
            itl->v += x;
        }
    }
    // 区间推平，全部赋值 x
    void assign_val(int l, int r, int64_t x) {
        // 划分区间，注意顺序，否则会引起 itl 迭代器失效
        auto itr = split(r), itl = split(l);
        s.erase(itl, itr);
        insert(l, r, x);
    }
    int64_t ranks(int l, int r, int k) {  // 区间第 k 小
        std::vector<std::pair<int64_t, int>> vp;
        for (auto itr = split(r), itl = split(l); itl != itr; ++itl) {
            vp.push_back({itl->v, itl->size()});
        }
        std::sort(vp.begin(), vp.end());
        for (auto it : vp) {
            k -= it.second;
            if (k <= 0) {
                return it.first;
            }
        }
        assert(false);
        return -1;
    }
    // 区间幂次和
    int64_t sum(int l, int r, int ex, int mod) {  
        auto powmod = [](int64_t a, int b, int mod) {
            int64_t ans = 1;
            for (a %= mod; b; b >>= 1, a = a * a % mod) {
                if (b & 1) {
                    ans = ans * a % mod;
                }
            }
            return ans;
        };

        int64_t res = 0;
        for (auto itr = split(r), itl = split(l); itl != itr; ++itl) {
            res = (res + itl->size() * powmod(itl->v, ex, mod)) % mod;
        }
        return res;
    }
};

const int mod = 1e9 + 7;

int seed, vmax;
int rnd() {
    int ret = seed;
    seed = (seed * 7LL + 13) % mod;
    return ret;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m >> seed >> vmax;

    Chtholly cho;
    for (int i = 0; i < n; ++i) {
        int x = rnd() % vmax + 1;
        cho.insert(i, i + 1, x);
    }

    while (m--) {
        int op = rnd() % 4 + 1;

        int l = rnd() % n;
        int r = rnd() % n;
        if (l > r) {
            std::swap(l, r);
        }
        r++;

        int64_t x, y;
        if (op == 3) {
            x = rnd() % (r - l) + 1;
        } else {
            x = rnd() % vmax + 1;
        }

        if (op == 4) {
            y = rnd() % vmax + 1;
        }

        if (op == 1) {
            cho.add(l, r, x);
        } else if (op == 2) {
            cho.assign_val(l, r, x);
        } else if (op == 3) {
            std::cout << cho.ranks(l, r, x) << "\n";
        } else {
            std::cout << cho.sum(l, r, x, y) << "\n";
        }
    }

    return 0;
}
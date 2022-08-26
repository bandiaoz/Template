#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct Chtholly {
    struct node {
        int l, r;
        mutable ll v;

        node(int l, int r, ll v) : l(l), r(r), v(v) {}
        int size() const {
            return r - l;
        }
        bool operator<(const node &A) const { 
            return l < A.l; 
        }
    };

    set<node> s;
    auto insert(int l, int r, ll v) {
        return s.insert(node(l, r, v));
    }
    auto split(int pos) {  //拆区间，将区间分为[l,pos), [pos,r)两段
        auto it = s.lower_bound(node(pos, -1, 0));
        if (it != s.end() && it->l == pos) {
            return it;
        }
        --it;
        int L = it->l, R = it->r;
        ll V = it->v;
        s.erase(it);
        insert(L, pos, V);
        //返回第二个区间的地址
        return insert(pos, R, V).first;
    }
    void add(int l, int r, ll x) {  //区间加
        for (auto itr = split(r), itl = split(l); itl != itr; ++itl) {
            itl->v += x;
        }
    }
    void assign_val(int l, int r, ll x) {  //区间推平，全部赋值x
        auto itr = split(r), itl = split(l);  //划分区间,注意顺序，否则会引起itl迭代器失效
        s.erase(itl, itr);
        insert(l, r, x);
    }
    ll ranks(int l, int r, int k) {  //区间第k小
        vector<pair<ll, int>> vp;
        for (auto itr = split(r), itl = split(l); itl != itr; ++itl) {
            vp.push_back({itl->v, itl->size()});
        }
        sort(vp.begin(), vp.end());
        for (auto it : vp) {
            k -= it.second;
            if (k <= 0) {
                return it.first;
            } 
        }
        assert(false);
        return -1;
    }
    ll sum(int l, int r, int ex, int mod) { //区间幂次和
        auto powmod = [](ll a, int b, int mod) {
            ll ans = 1;
            for (a %= mod; b; b >>= 1, a = a * a % mod) {
                if (b & 1) {
                    ans = ans * a % mod;
                }
            }
            return ans;
        };

        ll res = 0;
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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m >> seed >> vmax;

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
            swap(l, r);
        }
        r++;

        ll x, y;
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
            cout << cho.ranks(l, r, x) << "\n";
        } else {
            cout << cho.sum(l, r, x, y) << "\n";
        }
    }

    return 0;
}
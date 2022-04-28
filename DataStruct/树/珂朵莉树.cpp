#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;
typedef long long ll;
#define IT set<node>::iterator
const int MOD7 = 1e9 + 7;
const int maxn = 1e5 + 5;
struct node {
    int l, r;
    mutable ll v;
    node(int L, int R = -1, ll V = 0) : l(L), r(R), v(V) {}
    bool operator<(const node &A) const { return l < A.l; }
};
ll powmod(ll a, ll b, ll mod) {
    ll ans = 1;
    a %= mod;
    while (b) {
        if (b & 1) ans = ans * a % mod;
        b >>= 1;
        a = a * a % mod;
    }
    return ans;
}
set<node> s;
IT split(ll pos) {  //拆区间，将区间分为[l,pos-1],[pos,r]两段
    IT it = s.lower_bound(node(pos));
    if (it != s.end() && it->l == pos) return it;
    --it;
    int L = it->l, R = it->r;
    ll V = it->v;
    s.erase(it);
    s.insert(node(L, pos - 1, V));
    return s.insert(node(pos, R, V)).first;
    //返回第二个区间的地址
}
void add(int l, int r, ll x) {  //区间加
    IT itr = split(r + 1), itl = split(l);
    for (; itl != itr; ++itl) itl->v += x;
}
void assign_val(int l, int r, ll x) {  //区间推平，全部赋值x
    IT itr = split(r + 1),
       itl = split(l);  //划分区间,注意顺序，否则会引起itl迭代器失效
    s.erase(itl, itr);
    s.insert(node(l, r, x));
}
ll ranks(int l, int r, int k) {  //区间第k小
    vector<pair<ll, int>> vp;
    IT itr = split(r + 1), itl = split(l);  //划分区间
    vp.clear();
    for (; itl != itr; ++itl) vp.push_back({itl->v, itl->r - itl->l + 1});
    sort(vp.begin(), vp.end());
    for (auto it : vp) {
        k -= it.second;
        if (k <= 0) return it.first;
    }
    return 0;
}
ll sum(ll l, ll r, ll ex, ll mod) {         //区间幂次和,ai^x
    IT itr = split(r + 1), itl = split(l);  //划分区间
    ll res = 0;
    for (; itl != itr; ++itl)
        res = (res + (ll)(itl->r - itl->l + 1) * powmod(itl->v, ll(ex), ll(mod))) % mod;
    return res;
}
ll seed, vmax;
ll rnd() {
    ll ret = seed;
    seed = (seed * 7 + 13) % MOD7;
    return ret;
}
ll a[maxn];
int main() {
    int n, m;
    cin >> n >> m >> seed >> vmax;
    for (int i = 1; i <= n; ++i) {
        a[i] = (rnd() % vmax) + 1;
        s.insert(node(i, i, a[i]));
    }
    s.insert(node(n + 1, n + 1, 0));
    while (m--) {
        int op = (rnd() % 4) + 1;
        ll l = rnd() % n + 1;
        ll r = rnd() % n + 1;
        if (l > r) swap(l, r);
        ll x, y;
        if (op == 3)
            x = rnd() % (r - l + 1) + 1;
        else
            x = rnd() % vmax + 1;
        if (op == 4) y = rnd() % vmax + 1;
        if (op == 1)
            add(l, r, x);
        else if (op == 2)
            assign_val(l, r, ll(x));
        else if (op == 3)
            cout << ranks(l, r, x) << endl;
        else
            cout << sum(l, r, x, y) << endl;
    }
}
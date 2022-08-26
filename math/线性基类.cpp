#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct L_B {
    ll b[61], p[61];  //线性基，重构线性基
    int cnt, flag;    //重构线性基的大小,是否异或为0
    L_B() {
        memset(b, 0, sizeof(b));
        memset(p, 0, sizeof(p));
        cnt = 0, flag = 0;
    }
    inline bool insert(ll x) {
        for (int i = 60; i >= 0 && x; i--)
            if (x & (1LL << i)) {
                if (b[i]) x ^= b[i];
                else {
                    b[i] = x;
                    return true;
                }
            }
        flag = 1;
        return false;
    }
    ll qmax() {
        ll ans = 0;
        for (int i = 60; i >= 0; i--)
            if ((ans ^ b[i]) > ans) ans ^= b[i];
        return ans;
    }
    ll qmin() {
        if (flag) return 0;
        for (int i = 0; i <= 60; i++)
            if (b[i]) return b[i];
        return 0;
    }
    inline void rebuild() {
        for (int i = 60; i >= 1; i--) {
            if (b[i])
                for (int j = i - 1; j >= 0; j--)
                    if (b[i] & (1LL << j)) b[i] ^= b[j];
        }
        //异或p[i]对名次的贡献为1<<i
        for (int i = 0; i <= 60; i++)
            if (b[i]) p[cnt++] = b[i];
    }
    ll kth(ll k) {
        if (flag) --k;
        if (!k) return 0;
        ll ans = 0;
        if (k >= (1LL << cnt)) return -1;
        for (int i = 0; i <= cnt; ++i)
            if (k & (1LL << i)) ans ^= p[i];
        return ans;
    }
};
L_B merge(const L_B &n1, const L_B &n2) {
    L_B ans = n1;
    for (int i = 60; i >= 0; i--)
        if (n2.b[i]) ans.insert(n2.b[i]);
    ans.flag = n1.flag | n2.flag;
    return ans;
}
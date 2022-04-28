#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        a[i]--;
    }

    int q;
    cin >> q;
    vector<int> l(q), r(q);
    for (int i = 0; i < q; ++i) {
        cin >> l[i] >> r[i];
        l[i]--;
    }
    
    const int B = max(1.0, n / sqrt(q));
    vector<int> p(q);
    iota(p.begin(), p.end(), 0);
    sort(p.begin(), p.end(), [&](int i, int j) {
        if (l[i] / B == l[j] / B) return r[i] < r[j];
        else return l[i] < l[j];
    }); 

    vector<int> cnt(n);
    int L = 0, R = 0, res = 0;
    auto add = [&](int x, int f) {
        res -= cnt[x] / 2;
        cnt[x] += f;
        res += cnt[x] / 2;
    };

    vector<int> ans(q);
    for (auto i : p) {
        while (L > l[i]) add(a[--L], 1);
        while (R < r[i]) add(a[R++], 1);
        while (L < l[i]) add(a[L++], -1);
        while (R > r[i]) add(a[--R], -1);
        ans[i] = res;
    }

    for (int i = 0; i < q; ++i) {
        cout << ans[i] << "\n";
    }

    return 0;
}

// https://atcoder.jp/contests/abc242/tasks/abc242_g
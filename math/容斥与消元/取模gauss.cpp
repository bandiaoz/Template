ll a[55][55], x[55];
ll lcm(ll a, ll b) {
    return a / __gcd(a, b) * b;
}
ll pow2(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}
ll inv(ll a, ll m) {
    return pow2(a, mod - 2);
}
ll Gauss(ll m, ll n) {
    ll r = 0, c = 0;
    while (r < m && c < n) {
        ll id = r;
        for (ll i = r + 1; i < m; ++i)
            if (abs(a[i][c]) > abs(a[id][c]))
                id = i;
        if (id != r)
            for (ll i = 0; i <= n; ++i)
                swap(a[r][i], a[id][i]);
        if (abs(a[r][c]) != 0) {
            for (ll i = r + 1; i < m; ++i) {
                if (abs(a[i][c]) == 0) continue;
                ll LCM = lcm(abs(a[i][c]), abs(a[r][c]));
                ll ta = LCM / abs(a[i][c]);
                ll tb = LCM / abs(a[r][c]);
                if (a[i][c] * a[r][c] < 0) tb = -tb;
                for (ll j = c; j <= n; ++j)
                    a[i][j] = ((a[i][j] * ta - a[r][j] * tb) % mod + mod) % mod;
            }
            ++r;
        }
        ++c;
    }
    for (ll i = r; i < m; ++i)
        if (a[i][n] != 0) return -1;
    if (r < n) return n - r;  //矩阵的秩等于未知数即可，不需要等于m。
    for (ll i = n - 1; i >= 0; --i) {
        ll tmp = a[i][n];
        for (ll j = i + 1; j < n; ++j) {
            if (a[i][j] != 0) {
                tmp -= a[i][j] * x[j];
                tmp = (tmp % mod + mod) % mod;
            }
        }
        x[i] = (tmp * inv(a[i][i], mod)) % mod;
        debug(i, x[i])
    }
    return 0;
}

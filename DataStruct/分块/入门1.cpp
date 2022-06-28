#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    const int B = sqrt(n); // [id * B, (id + 1) * B)
    vector<ll> a(n), base(B + 1), res(B + 1); 

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        res[i / B] += a[i];
    }

    auto update = [&](int l, int r, ll c) {
        int idl = l / B, idr = r / B;
        if (idl == idr) {
            for (int i = l; i <= r; ++i) {
                a[i] += c;
                res[idl] += c;
            }
        } else {
            for (int i = l; i < (idl + 1) * B; ++i) {
                a[i] += c;
                res[idl] += c;
            }

            for (int id = idl + 1; id <= idr - 1; ++id) {
                res[id] += c * B;
                base[id] += c;
            }

            for (int i = idr * B; i <= r; ++i) {
                a[i] += c;
                res[idr] += c;
            }
        }
    };

    for (int i = 0; i < n; ++i) {
        int op, l, r;
        ll c;
        cin >> op >> l >> r >> c;

        if (op == 1) {
            r--;
            cout << a[r] + base[r / B] << "\n";
        } else {
            l--, r--;
            update(l, r, c);
        }
    }

    return 0;
}

// test problem: https://loj.ac/p/6277
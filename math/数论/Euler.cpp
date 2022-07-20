#include <bits/stdc++.h>

using namespace std;
using ll = long long;

// 表示的是小于等于n和n互质的数的个数。
int euler_phi(int n) {
    int ans = n;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) {
            ans = ans / i * (i - 1);
            while (n % i == 0) n /= i;
        }
    if (n > 1) ans = ans / n * (n - 1);
    return ans;
}

vector<int> phi_table(int n) {
    vector<int> phi(n + 1);
    phi[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (phi[i]) continue;
        for (int j = i; j <= n; j += i) {
            if (!phi[j]) phi[j] = j;
            phi[j] = phi[j] / i * (i - 1);
        }
    }
    return phi;
}
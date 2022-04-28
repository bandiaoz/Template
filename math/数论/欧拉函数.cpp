#pragma region
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <vector>
typedef long long ll;
#define IO ios::sync_with_stdio(false), cin.tie(0), cout.tie(0)
#define rep(i, a, n) for (ll i = a; i <= n; i++)
using namespace std;
#pragma endregion

int euler_phi(int n) {
    int m = int(sqrt(n + 0.5));
    int ans = n;
    for (int i = 2; i <= m; i++)
        if (n % i == 0) {
            ans = ans / i * (i - 1);
            while (n % i == 0) n /= i;
        }
    if (n > 1) ans = ans / n * (n - 1);
    return ans;
}

void phi_table(int n, int* phi) {
    for (int i = 2; i <= n; i++) phi[i] = 0;
    phi[1] = 1;
    for (int i = 2; i <= n; i++)
        if (!phi[i])
            for (int j = i; j <= n; j += i) {
                if (!phi[j]) phi[j] = j;
                phi[j] = phi[j] / i * (i - 1);
            }
}
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // n / l = n / (l + 1) = ... = n / r,1 <= l <= r <= k
    auto block = [&](int n, int k) {
        vector<array<int, 2>> ans;
        for (int l = 1, r; l <= k; l = r + 1) {
            r = (n / l ? min(k, n / (n / l)) : k);
            ans.push_back({l, r});
        }
        for (auto [l, r] : ans) {
            cout << l << " " << r << " " << n / l << "\n";
        }
    };

    block(24, 24);

    return 0;
}

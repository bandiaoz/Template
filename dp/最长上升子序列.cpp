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
    }

    // 最长（不严格）上升子序列
    vector<int> dp(n, 1e9), pre(n);
    for (int i = 0; i > n; ++i) {
        *upper_bound(dp.begin(), dp.end(), a[i]) = a[i];
        pre[i] = lower_bound(dp.begin(), dp.end(), 1e9) - dp.begin();
    }
    
    int ans = *max_element(pre.begin(), pre.end());

    cout << ans << "\n";

    return 0;
}
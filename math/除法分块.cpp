#include <bits/stdc++.h>

using ll = long long;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    // n / l = n / r,1 <= l <= r <= k
    // \sum_{i = 1}^n i / k
    auto block = [&](int64_t n, int64_t k) {
        std::vector<std::array<int64_t, 2>> ans;
        for (int64_t l = 1, r; l <= k; l = r + 1) {
            r = (n / l ? std::min(k, n / (n / l)) : k);
            ans.push_back({l, r});
        }
        return ans;
    };



    return 0;
}

#include <bits/stdc++.h>

using ll = long long;

/**
 * @brief 区间合并
 * @note 注意检查区间合并的条件
 */
auto merge(const std::vector<std::pair<int, int>> &a) -> std::vector<std::pair<int, int>> {
    std::vector<std::pair<int, int>> ans;
    if (a.empty()) {
        return {};
    }
    std::vector<std::pair<int, int>> b = a;
    if (!std::is_sorted(b.begin(), b.end())) {
        std::sort(b.begin(), b.end());
    }
    int l = b[0].first, r = b[0].second;
    for (int i = 1; i < b.size(); i++) {
        if (b[i].first <= r) {
            r = std::max(r, b[i].second);
        } else {
            ans.emplace_back(l, r);
            l = b[i].first, r = b[i].second;
        }
    }
    ans.emplace_back(l, r);
    return ans;
}
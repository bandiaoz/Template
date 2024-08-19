#include <bits/stdc++.h>

using ll = long long;

/**
 * @brief merge intervals
 * @note 注意检查区间合并的条件
 * @param sorted(whether a is sorted, if not then sort it)
 * @return merged intervals
 */
auto merge(const std::vector<std::pair<int, int>> &a, bool sorted = true) -> std::vector<std::pair<int, int>> {
    std::vector<std::pair<int, int>> ans;
    if (a.empty()) {
        return ans;
    }
    std::vector<std::pair<int, int>> b = a;
    if (!sorted) {
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
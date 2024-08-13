#include <algorithm>
#include <vector>

/**
 * @brief 计算数组 v 的逆序对数，即有多少对 (i, j) 满足 i < j 且 v[i] > v[j]
 */
template<typename T>
int64_t count_inversion(const std::vector<T>& v) {
    std::vector<T> a(v);
    std::vector<T> tmp(v.size());
    int64_t ans{};
    auto merge = [&](auto &&self, int l, int r) {
        if (r - l <= 1) {
            return;
        }
        int mid = l + (r - l) / 2;
        self(self, l, mid);
        self(self, mid, r);
        for (int i = l, j = mid, k = l; k < r; k++) {
            if (j == r || (i < mid && a[i] <= a[j])) {
                tmp[k] = a[i++];
            } else {
                tmp[k] = a[j++];
                ans += mid - i;
            }
        }
        std::copy(tmp.begin() + l, tmp.begin() + r, a.begin() + l);
    };
    merge(merge, 0, (int)v.size());
    return ans;
}
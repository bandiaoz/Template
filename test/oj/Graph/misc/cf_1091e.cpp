#include <bits/stdc++.h>

using ll = long long;

/*
[CF1091E New Year and the Acquaintance Estimation](https://codeforces.com/contest/1091/problem/E)
[status](https://codeforces.com/contest/1091/submission/328346766)
*/
/**
 * 给定长度为 n 的度数序列，第 n+1 个点的度数可以是多少，才能组成 n+1 个点的简单图
 * 
 * Erdős–Gallai 定理用于判断一个非递增的度数序列是否可以构成一个简单图
 * 对于 $$\{d_1, d_2, \cdots, d_n\}$$，$$d_i \geq d_{i+1}$$，这个序列可简单化当且仅当，对于 $$\forall 1 \leq k \leq n$$，都有 $$\sum_{i = 1}^k d_i \leq k(k - 1) + \sum_{i = k + 1}^n \min(d_i, k)$$
 * 对于给定的非递增度数序列，可以 $$O(n)$$ 判断
 * 
 * 这题中，首先第 n+1 个点的度数的奇偶性是确定的（$$\sum deg[i]$$ 一定是偶数），并且可行的答案一定是连续的，可以二分
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> deg(n);
    for (auto &i : deg) {
        std::cin >> i;
    }

    std::sort(deg.begin(), deg.end(), std::greater<int>());
    int parity = std::accumulate(deg.begin(), deg.end(), 0LL) & 1;

    auto judge = [&](int x) -> int {
        std::vector<int> c(n + 1);
        for (auto i : deg) {
            c[i]++;
        }
        c[x]++;

        int less = 0;
        ll left = 0, right = 0;
        for (int k = 0, i = 0; k <= n; k++) {
            int val = (i == k && (i == n || deg[i] < x)) ? x : deg[i++];
            left += val;
            c[val]--;
            right -= std::min(val, k);
            less += c[k];
            right += n - k - less;
            if (left > right + 1LL * (k + 1) * k) {
                return (i == k) ? 1 : -1;
            }
        }
        return 0;
    };

    int min = -1, max = -1;
    int l = 0, r = (n - parity) / 2;
    while (l < r) {
        int mid = (l + r) / 2;
        if (judge(2 * mid + parity) == -1) {
            l = mid + 1;
        } else {
            r = mid;
        }
    }
    min = l;

    l = 0, r = (n - parity) / 2;
    while (l < r) {
        int mid = (l + r + 1) / 2;
        if (judge(2 * mid + parity) == 1) {
            r = mid - 1;
        } else {
            l = mid;
        }
    }
    max = l;

    if (judge(2 * min + parity) || judge(2 * max + parity)) {
        std::cout << "-1\n";
    } else {
        for (int i = min; i <= max; i++) {
            std::cout << 2 * i + parity << " \n"[i == max];
        }
    }

    return 0;
}
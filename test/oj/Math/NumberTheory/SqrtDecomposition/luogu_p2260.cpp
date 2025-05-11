#include <bits/stdc++.h>
#include "src/Math/NumberTheory/SqrtDecomposition.h"
#include "src/Math/NumberTheory/PairSqrtDecomposition.h"
#include "src/Math/Modular/StaticModInt32.h"

using Z = OY::StaticModInt32<19940417, false>;

/*
[P2260 [清华集训 2012] 模积和](https://www.luogu.com.cn/problem/P2260)
[status](https://www.luogu.com.cn/record/210290281)
*/
/**
 * 求 $$\sum_{i=1}^{n} \sum_{j=1}^{m} (n \bmod i) \times (m \bmod j), i \neq j$$
 * 
 * 拆成两个部分
 * 
 * $$ans = \sum_{i=1}^{n} \sum_{j=1}^{m} (n \bmod i) \times (m \bmod j) - \sum_{i=1}^{min} (n \bmod i) \times (m \bmod i)$$
 * 
 * 将两个求和拆开
 * 
 * $$ans = \sum_{i=1}^{n}  (n \bmod i) \times \sum_{j=1}^{m} (m \bmod j) - \sum_{i=1}^{min} (n \bmod i) \times (m \bmod i)$$
 * 
 * 利用 $$(n \bmod i) = n - i \times \lfloor \frac{n}{i} \rfloor$$
 * 
 * $$ans = \sum_{i=1}^{n}  \left(n - i \times \lfloor \frac{n}{i} \rfloor\right) \times \sum_{j=1}^{m} \left(m - j \times \lfloor \frac{m}{j} \rfloor\right) - \sum_{i=1}^{min} \left(n - i \times \lfloor \frac{n}{i} \rfloor\right) \times \left(m - i \times \lfloor \frac{m}{i} \rfloor\right)$$
 * 
 * 
 * $$f(n, max) = \sum_{i=1}^{max} \left(i \times \lfloor \frac{n}{i} \rfloor\right)$$ 
 * 
 * 
 * $$g(n, m, max) =\sum_{i = 1}^{max} \left(i^2 \times \lfloor \frac{n}{i} \rfloor \times \lfloor \frac{m}{i} \rfloor\right)$$ 
 * 
 * 故可得 $$ans = \left(n^2 - f(n, n)\right) \times \left(m^2 - f(m, m)\right) - \left(nm - n \times f(m, min) - m \times f(n, min) + g(n, m, min)\right)$$
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    auto f = [&](int n, int max) {
        Z ans = 0;
        OY::SqrtDecomposition<uint32_t> sd(n);
        for (auto range = sd.rbegin(); range != sd.rend(); ++range) {
            int left = range->left(), right = range->right();
            if (left > max) break;
            right = std::min<int>(right, max);
            int len = right - left + 1;
            ans += Z(1) * range->quot() * (left + right) * len / 2;
        }
        return ans;
    };
    auto get_sum = [&](int n) {
        return Z(1) * n * (n + 1) * (2 * n + 1) / 6;
    };
    auto g = [&](int n, int m, int max) {
        Z ans = 0;
        OY::PairSqrtDecomposition<uint32_t> sd(n, m);
        for (auto range = sd.rbegin(); range != sd.rend(); ++range) {
            int left = range.left(), right = range.right();
            if (left > max) break;
            right = std::min<int>(right, max);
            int len = right - left + 1;
            ans += Z(1) * range.quot1() * range.quot2() * (get_sum(right) - get_sum(left - 1));
        } 
        return ans;
    };

    int min = std::min(n, m);
    Z sum = (Z(1) * n * n - f(n, n)) * (Z(1) * m * m - f(m, m));
    Z sub = Z(1) * min * n * m - n * f(m, min) - m * f(n, min) + g(n, m, min);

    std::cout << sum - sub << "\n";

    return 0;
}
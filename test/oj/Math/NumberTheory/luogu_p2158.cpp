#include <bits/stdc++.h>
#include "src/Math/NumberTheory/EulerSieve.h"

using ll = long long;

/*
[P2158 SDOI2008 仪仗队](https://www.luogu.com.cn/problem/P2158)
[status](https://www.luogu.com.cn/record/229157769)
*/
/*
给定一个 $$n \times n$$ 的方阵，求有多少个点对 $$(i, j)$$ 满足 $$i \neq j$$ 且 $$i, j$$ 互质。
$$1 \leq n \leq 40000$$

前两列特判，一共有 3 个点，分别是 $$(0, 0), (0, 1), (1, 0)$$
第三列开始，每一列贡献的点数为 $$2 \times \varphi(i - 1)$$，其中 $$i$$ 为列数
*/

void solve_phi() {
    OY::EulerSieve::Sieve<40'000, true> sieve;

    int n;
    std::cin >> n;

    if (n == 1) {
        std::cout << "0\n";
        return;
    }

    ll ans = 0;
    for (int i = 2; i < n; i++) {
        ans += sieve.get_Euler_Phi(i) * 2;
    }
    std::cout << ans + 3 << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_phi();

    return 0;
}
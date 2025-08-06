#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"
#include "src/Math/LinearAlgebra/DynamicMatrix.h"

using ll = long long;

/*
[CF696D Legen...](https://codeforces.com/contest/696/problem/D)
[status](https://codeforces.com/contest/696/submission/329169537)
*/
/*
给定 n 个模式串 $$s_i$$ 和数组 $$a_i$$，一个字符串 T 的价值为 $$\sum_{i = 1}^n a_i \times \text{cnt}(T, s_i)$$，其中 $$\text{cnt}(T, s_i)$$ 表示 $$s_i$$ 在 $$T$$ 中出现的次数
输出长度为 l 的字符串的最大价值
$$1 \leq n \leq 200, 1 \leq l \leq 10^{14}, 1 \leq \sum |s_i| \leq 200, 1 \leq a_i \leq 100$$

dp[i][j] 表示长度为 i 的字符串，在 ac 自动机上走到 j 的最大价值，用矩阵快速幂优化，矩阵乘法重载为 max-plus 乘法
$$s_i$$ 在 fail 树上对子树做贡献，走到子树的每个点，都可以获得 $$a_i$$ 的贡献
时间复杂度 $$O(N^3 \log l)$$，其中 $$N = \sum |s_i|$$
*/

auto mul = [](const OY::DynamicMatrix<ll> &a, const OY::DynamicMatrix<ll> &b) {
    assert(a.column() == b.row());
    constexpr ll INF_NEG = std::numeric_limits<ll>::min() / 2;
    auto res = OY::DynamicMatrix<ll>::raw(a.row(), b.column(), INF_NEG);
    for (uint32_t i = 0; i < a.row(); i++) {
        for (uint32_t k = 0; k < a.column(); k++) {
            for (uint32_t j = 0; j < b.column(); j++) {
                res[i][j] = std::max(res[i][j], a[i][k] + b[k][j]);
            }
        }
    }
    return res;
};
auto power = [](OY::DynamicMatrix<ll> a, ll b) {
    assert(a.row() == a.column());
    auto res = OY::DynamicMatrix<ll>::raw(a.row(), a.column(), 0);
    for (; b; b >>= 1, a = mul(a, a)) {
        if (b & 1) res = mul(res, a);
    }
    return res;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    ll l;
    std::cin >> n >> l;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }
    const int N = 200;
    std::vector<std::string> s(n);
    OY::ACAM<26> ac;
    ac.reserve(N);
    std::vector<int> pos(n);
    for (int i = 0; i < n; i++) {
        std::cin >> s[i];
        pos[i] = ac.insert_lower(s[i]);
    }

    ac.prepare();

    std::vector<ll> val(ac.m_data.size());
    for (int i = 0; i < n; i++) {
        val[pos[i]] += a[i];
    }
    ac.do_for_extending_nodes([&](int u) {
        val[u] += val[ac.query_fail(u)];
    });
    
    const ll inf = 1e18;
    OY::DynamicMatrix<ll> mat(ac.m_data.size(), ac.m_data.size(), -inf);
    for (int i = 0; i < ac.m_data.size(); i++) {
        for (int c = 0; c < 26; c++) {
            int j = ac.next(i, c);
            mat[j][i] = std::max(mat[j][i], val[j]);
        }
    }

    OY::DynamicMatrix<ll> vec(ac.m_data.size(), 1, -inf);
    vec[0][0] = 0;

    auto res = mul(power(mat, l), vec);
    ll ans = -inf;
    for (int i = 0; i < ac.m_data.size(); i++) {
        ans = std::max(ans, res[i][0]);
    }
    std::cout << ans << '\n';

    return 0;
}
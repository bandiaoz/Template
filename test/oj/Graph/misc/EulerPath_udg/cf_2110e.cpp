#include <bits/stdc++.h>
#include "src/DataStruct/misc/Discretizer.h"
#include "src/Graph/misc/EulerPath_udg.h"
#ifndef LOCAL
#define debug(...)
#endif

using ll = long long;

/*
[P4381 [E. Melody](https://codeforces.com/contest/2110/problem/E)
[status](https://codeforces.com/contest/2110/submission/326620542)
*/
/**
 * 给定 n 个数对 (a_i, b_i)，要求重排，相邻的数对要么 ai 相等，要么 bi 相等，并且不存在 a_i = a_{i+1} = a_{i+2} 或者 b_i = b_{i+1} = b_{i+2}
 * 
 * 构建二分图，a 和 b 分别作为左右部点，(a_i, b_i) 作为边，则问题转化为构造一个欧拉路径，使得每条边只经过一次
 */

void solve() {
    int n;
    std::cin >> n;
    std::vector<int> a(n), b(n);
    OY::Discretizer<int> Da, Db;
    for (int i = 0; i < n; i++) {
        std::cin >> a[i] >> b[i];
        Da << a[i];
        Db << b[i];
    }

    Da.prepare();
    Db.prepare();

    OY::EulerPathUDG::Graph graph(Da.size() + Db.size());
    for (int i = 0; i < n; i++) {
        a[i] = Da.rank(a[i]);
        b[i] = Db.rank(b[i]) + Da.size();
        graph.add_edge(a[i], b[i]);
    }

    auto sol = graph.calc();
    if (sol.is_half_Euler_graph() || sol.is_Euler_graph()) {
        std::cout << "YES\n";
        sol.trace([&](int i, int u, int v) {
            std::cout << i + 1 << " ";
        });
        std::cout << "\n";
    } else {
        std::cout << "NO\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
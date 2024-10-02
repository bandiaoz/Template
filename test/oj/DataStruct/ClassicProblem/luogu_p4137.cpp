#include <bits/stdc++.h>
#include "src/DataStruct/ClassicProblem/RangeMex.h"
#include "src/DataStruct/container/StaticBufferWrapWithoutCollect.h"

using ll = long long;

/*
[P4137 Rmq Problem / mex](https://www.luogu.com.cn/problem/P4137)
*/
/**
 * 离线求区间 mex 模板
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    OY::RangeMex<OY::StaticBufferWrapWithoutCollect<5000000>::type> S(n, [&](...) {
        int x;
        std::cin >> x;
        return x;
    });

    while (q--) {
        int l, r;
        std::cin >> l >> r;
        l--;
        std::cout << S.query(l, r) << '\n';
    }

    return 0;
}
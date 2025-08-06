#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt32.h"
#include "src/Math/Poly/LagrangeInterpolation.h"

using Z = OY::mint998244353;

/*
[P4781 【模板】拉格朗日插值](https://www.luogu.com.cn/problem/P4781)
[status](https://www.luogu.com.cn/record/183385361)
*/
/**
 * 本题为拉格朗日插值的模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k;
    std::cin >> n >> k;

    OY::LagrangeInterpolation<Z> li(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        li.addPoint(x, y);
    }

    li.prepare();
    std::cout << li.calc(k) << '\n';
    
    return 0;
}
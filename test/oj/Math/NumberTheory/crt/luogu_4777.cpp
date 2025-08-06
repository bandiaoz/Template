#include <bits/stdc++.h>
#include "src/Math/NumberTheory/ChineseRemainderTheorem_ex.h"

using ll = long long;

/*
[P4777 【模板】扩展中国剩余定理（EXCRT）](https://www.luogu.com.cn/problem/P4777)
[status](https://www.luogu.com.cn/record/183909973)
*/
/**
 * 本题为扩展中国剩余定理模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::ChineseRemainderTheorem_ex crt;
    for (int i = 0; i < n; i++) {
        ll div, rem;
        std::cin >> div >> rem;
        crt.add(rem, div);
    }

    std::cout << crt.query().m_rem << '\n';

    return 0;
}
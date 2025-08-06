#include <bits/stdc++.h>
#include "src/Math/NumberTheory/ChineseRemainderTheorem.h"

/*
[P3868 [TJOI2009] 猜数字](https://www.luogu.com.cn/problem/P3868)
[status](https://www.luogu.com.cn/record/229146470)
*/
/*
给定两个长度为 $$n$$ 的数组 $$a$$ 和 $$b$$，求最小的 $$x \in \mathbb{N}$$ 满足 $$b_i \mid (x - a_i)$$ 对所有 $$i$$ 成立。

$$b_i \mid (x - a_i) \iff x \equiv a_i \pmod{b_i}$$
使用中国剩余定理求解。
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n), b(n);
    for (auto &i : a) {
        std::cin >> i;
    }
    for (auto &i : b) {
        std::cin >> i;
    }
    
    OY::ChineseRemainderTheorem crt(n);
    for (int i = 0; i < n; i++) {
        a[i] = (a[i] % b[i] + b[i]) % b[i];
        crt.add(a[i], b[i]);
    }

    std::cout << crt.query().m_rem << '\n';
    

    return 0;
}
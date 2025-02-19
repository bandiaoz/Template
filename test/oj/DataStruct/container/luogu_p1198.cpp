#include <bits/stdc++.h>
#include "src/DataStruct/container/MonoStack.h"

using ll = long long;

/*
[P1198 [JSOI2008] 最大数](https://www.luogu.com.cn/problem/P1198)
[status](https://www.luogu.com.cn/record/202937964)
*/
/**
 * 维护一个数组，支持插入元素，查询末尾 L 个数中的最大值
 * 
 * 考虑到区间查询的右边界始终在右移，所以可以用单调栈+二分解决
 */

void solve_monostack() {
    int m, P;
    std::cin >> m >> P;

    int last = 0;
    auto getter = [&](...) {
        ll x;
        std::cin >> x;
        return (x + last) % P;
    };

    OY::MonoMaxStack<int, decltype(getter)> stk(m, {}, getter, {}, P, true);
    while (m--) {
        char op;
        std::cin >> op;

        if (op == 'A') {
            stk.extend_right();
        } else {
            int len;
            std::cin >> len;
            last = stk.raw_query_on_left_side(stk.cursor() - len + 1)->m_value;
            std::cout << last << '\n';
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_monostack();

    return 0;
} 
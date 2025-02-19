#include <bits/stdc++.h>
#include "src/DataStruct/container/MonoStack.h"

/*
[P5788 【模板】单调栈](https://www.luogu.com.cn/problem/P5788)
[status](https://www.luogu.com.cn/record/202938016)
*/
/**
 * 定义 f(i) 为 a[i] 右侧第一个大于 a[i] 的元素的下标，不存在则 f(i) = 0
 * 
 * 考虑单调栈，维护一个非严格单调递减栈
 * 如果 push a[i] 时，将栈顶元素 a[j] pop，则 f(j) = i
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    auto getter = [&](auto ...) {
        int x;
        std::cin >> x;
        return x;
    };

    std::vector<int> ans(n);
    auto pop_call = [&](int popped, int left_smaller) {
        ans[popped] = left_smaller + 1;
    };

    OY::MONOSTACK::Table<int, std::less_equal<int>, decltype(getter), decltype(pop_call)> stk(
        n, {}, getter, pop_call, std::numeric_limits<int>::max()
    );
    for (int i = 0; i < n; i++) {
        stk.extend_right();
    }
    for (int i = 0; i < n; i++) {
        std::cout << ans[i] << " \n"[i == n - 1];
    }

    return 0;
}
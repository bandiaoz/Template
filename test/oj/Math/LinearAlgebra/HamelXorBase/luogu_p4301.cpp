#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBase.h"

using ll = long long;

/*
[P4301 [CQOI2013] 新Nim游戏](https://www.luogu.com.cn/problem/P4301)
[status](https://www.luogu.com.cn/record/207059260)
*/
/**
 * 对于传统 nim 游戏，每堆石子的异或和为 0 则先手必败
 * 新规则是，第一回合可以拿走任意堆石子，可以不拿，但是不能全部拿走
 * 问先手第一回合怎么拿可以必胜，输出最少拿走多少石子
 * 
 * 先手需要保证，拿走若干堆石子后，后手剩余不能组成任何异或和为 0 的集合
 * 那么先手拿完剩余一个线性基即可，从大到小排序，无法插入就是先手需要拿走的
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    std::sort(a.begin(), a.end(), std::greater<int>());

    ll ans = 0;
    OY::StaticHamelXorBase64<30> hxb;
    for (auto i : a) {
        if (!~hxb.insert(i)) {
            ans += i;
        }
    }

    std::cout << ans << "\n";

    return 0;
}
#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBase.h"

/*
[CF1163E Magical Permutation](https://codeforces.com/contest/1163/problem/E)
[status](https://codeforces.com/contest/1163/submission/343808008)
*/
/*
给定长度为 $$n(1 \leq n \leq 2 \times 10^5)$$ 的数组，要求构造一个长度为 $$2^x$$ 的排列(排列元素由 0 到 $$2^x - 1$$ 组成)，使得排列中任意两个相邻元素的异或值为数组中的元素
要求最大化排列的长度，并输出排列

首先排列的最高位 $$2^{x - 1}$$ 一定需要小于等于数组的最大值，即 $$2^{x - 1} \leq \max(a)$$，因为相邻异或一定会包含这个最高位
能构造长度为 $$2^x$$ 的排列，充要条件是 $$n$$ 个数构成的线性基可以表示出 $$0$$ 到 $$2^x - 1$$ 的所有数
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

    int max = std::ranges::max(a);
    auto check = [&](int x) -> bool {
        OY::StaticHamelXorBase32<20> hxb;
        std::vector<int> number;
        for (int i = 0; i < n; i++) {
            if (a[i] < (1 << x)) {
                if (hxb.insert(a[i]) != -1) {
                    number.push_back(a[i]);
                }
            }
        }
        if (hxb.base_count() != x) {
            return false;
        }
        
        std::vector<int> stk{0};
        std::vector<int> vis(1 << x);
        std::vector<int> ans;
        while (!stk.empty()) {
            int cur = stk.back();
            stk.pop_back();
            if (vis[cur]) {
                continue;
            }
            vis[cur] = 1;
            ans.push_back(cur);
            for (int num : number) {
                if (!vis[num ^ cur]) {
                    stk.push_back(num ^ cur);
                }
            }
        }

        std::cout << x << '\n';
        for (int i = 0; i < ans.size(); i++) {
            std::cout << ans[i] << " \n"[i == ans.size() - 1];
        }
        return true;
    };

    for (int x = 20; x >= 1; x--) {
        if ((1 << (x - 1)) <= max && check(x)) {
            return 0;
        }
    }

    std::cout << "0\n0\n";

    return 0;
} 
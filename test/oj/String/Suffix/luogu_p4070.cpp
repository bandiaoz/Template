#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"
#include "src/DataStruct/Segtree/CatTree.h"

using ll = long long;

/*
[P4070 [SDOI2016] 生成魔咒](https://www.luogu.com.cn/problem/P4070)
[status(SA)](https://www.luogu.com.cn/record/192483578)
*/
/**
 * 动态字符串，每次在字符串末尾加入一个字符，求不同子串个数
 * SA 做法：将字符串反转，转换为每次在字符串开头加入一个字符，求不同子串个数；求 height 数组的区间 min 来计算重复子串个数的贡献
 */

void solve_sa() {
    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    OY::SA::SuffixArray<true, true, 100000> sa(a.rbegin(), a.rend());
    OY::CatMinTable<int> cat_tree(n, [&](int i) { return sa.query_height(i); });
    std::set<int> st;

    ll same = 0;
    for (int i = n - 1; i >= 0; i--) {
        int rnk = sa.query_rank(i);
        st.insert(rnk);
        auto prev = st.lower_bound(rnk);
        auto next = st.upper_bound(rnk);
        int pre = prev == st.begin() ? -1 : *std::prev(prev);
        int nxt = next == st.end() ? -1 : *next;
        if (pre != -1 && nxt != -1) {
            same += cat_tree.query(pre + 1, rnk + 1);
            same += cat_tree.query(rnk + 1, nxt + 1);
            same -= cat_tree.query(pre + 1, nxt + 1);
        } else if (pre != -1 && nxt == -1) {
            same += cat_tree.query(pre + 1, rnk + 1);
        } else if (pre == -1 && nxt != -1) {
            same += cat_tree.query(rnk + 1, nxt + 1);
        }
        // 当前所有子串个数 - 重复子串个数
        std::cout << 1LL * (n - i) * (n - i + 1) / 2 - same << '\n';
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_sa();

    return 0;
}
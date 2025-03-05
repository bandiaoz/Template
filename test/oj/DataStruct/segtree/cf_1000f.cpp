#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/ZkwTree.h"

/*
[CF1000F One Occurrence](https://codeforces.com/contest/1000/problem/F)
[status](https://codeforces.com/contest/1000/submission/308746002)
*/
/**
 * 给定长度为 n 的数组 a，q 次查询，每次查询区间 [l, r] 的哪个数只出现过一次，输出这个数
 * 不存在则输出 0
 * 
 * 将询问离线下来按右端点排序，如果区间 [l, r] 内每个数最后一次出现下标为 p，并且 prev[p] 不存在或者 prev[p] < l，则这个数在区间中唯一出现
 * 其中 prev[p] 在 p 左边并且值等于 a[p] 的最大位置
 */

using Info = int;
auto op = [](Info a, Info b) { return std::min(a, b); };

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    std::map<int, int> pos;
    std::vector<int> prev(n, -1);
    for (int i = 0; i < n; i++) {
        if (pos.contains(a[i])) {
            prev[i] = pos[a[i]];
        }
        pos[a[i]] = i;
    } 

    int q;
    std::cin >> q;
    std::vector<std::array<int, 3>> query(q);
    for (int i = 0; i < q; i++) {
        int l, r;
        std::cin >> l >> r;
        l--;
        query[i] = {l, r, i};
    }

    std::sort(query.begin(), query.end(), [&](const auto &lhs, const auto &rhs) {
        return lhs[1] < rhs[1];
    });

    const int inf = 1e9;
    auto zkw = OY::make_ZkwTree<Info>(n, op, [](...) { return inf; });
    std::vector<int> ans(q);
    for (int i = 0; auto [l, r, id] : query) {
        while (i < r) {
            zkw.modify(i, prev[i]);
            if (prev[i] != -1) {
                zkw.modify(prev[i], inf);
            }
            i++;
        }
        if (int min = zkw.query(l, r); min < l) {
            int p = zkw.max_right(l, [&](Info x) { return x > min; }) + 1;
            ans[id] = a[p];
        }
    } 

    for (auto i : ans) {
        std::cout << i << "\n";
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/DataStruct/misc/RangeManger.h"
#include "src/DataStruct/Segtree/ChthollySegTree.h"
#include "src/DataStruct/Segtree/ChthollyZkwTree.h"
#include "src/DataStruct/Bit/LazyBitset.h"

/*
[P3740 [HAOI2014] 贴海报](https://www.luogu.com.cn/problem/P3740)
[status(range_manager)](https://www.luogu.com.cn/record/204619513)
[status(chtholly_seg_tree)](https://www.luogu.com.cn/record/204621352)
[status(chtholly_zkw_tree)](https://www.luogu.com.cn/record/204621519)
[status(lazy_bitset)](https://www.luogu.com.cn/record/204622473)
*/
/**
 * 按照顺序张贴海报，前面的海报可能会被后面的海报覆盖，问最后可以看见多少张海报
 * 
 * 本题为 RangeManager/ChthollySegTree 模板题
 * 倒序插入每个区间，如果某个区间插入时，该区间已经全部存在，说明这个区间最终被完全覆盖
 */

void solve_range_manger() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::pair<int, int>> colors(m);
    for (auto &[l, r] : colors) {
        std::cin >> l >> r;
        l--;
    }

    OY::RangeManager<int> ranges;
    int ans = 0;
    for (auto &range : colors | std::views::reverse) {
        if (ranges.all_of(range) == ranges.end()) {
            ranges.add_range(range);
            ans++;
        }
    }

    std::cout << ans << '\n';
}

void solve_chtholly_tree() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::pair<int, int>> colors(m);
    for (auto &[l, r] : colors) {
        std::cin >> l >> r;
        l--;
    }

    OY::VectorChthollySegTree<bool, uint32_t> cho(n);
    // OY::ChthollyZkwTree<bool> cho(n);
    int ans = 0;
    for (auto &[l, r] : colors | std::views::reverse) {
        if (auto val = cho.query(l, r); val.has_change() || val.get() == false) {
            ans++;
            cho.modify(l, r, true);
        }
    }

    std::cout << ans << "\n";
}

void solve_bitset() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::pair<int, int>> colors(m);
    for (auto &[l, r] : colors) {
        std::cin >> l >> r;
        l--;
    }
    
    OY::VectorLazyBitset<uint32_t> bitset(n);
    int ans = 0;
    for (auto &[l, r] : colors | std::views::reverse) {
        if (!bitset.all(l, r)) {
            ans++;
            bitset.set(l, r);
        }
    }

    std::cout << ans << "\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // solve_range_manger();
    // solve_chtholly_tree();
    solve_bitset();

    return 0;
}
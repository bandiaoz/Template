#include <bits/stdc++.h>
#include "src/DataStruct/RMQ/MaskRMQ.h"

using ll = long long;

/*
[P3865 【模板】ST 表](https://www.luogu.com.cn/problem/P3865)
[status(STTable)](https://www.luogu.com.cn/record/182904231)
[status(MaskRMQ)](https://www.luogu.com.cn/record/182930658)
*/
/**
 * 本题要进行区间最值查询，为 RMQ 模板题
 * 可以使用 MaskRMQ SqrtTree STTable CatTree ZkwTree SegTree 等数据结构
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    auto read = [&](...) {
        int x;
        std::cin >> x;
        return x;
    };
    // OY::STMaxTable<int> max(n, read);
    OY::MaskRMQMaxValueTable<int> max(n, read);

    while (m--) {
        int l, r;
        std::cin >> l >> r;
        l--;
        std::cout << max.query(l, r) << '\n';
    }

    return 0;
}
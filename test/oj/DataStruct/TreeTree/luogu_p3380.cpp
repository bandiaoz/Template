#include <bits/stdc++.h>
#include "src/DataStruct/TreeTree/SegBit.h"

using ll = long long;

/*
[P3380 【模板】树套树](https://www.luogu.com.cn/problem/P3380)
[status](https://www.luogu.com.cn/record/187077005)
*/
/**
 * 本题类似 P2617
 * 不妨在一个二维 01 数组上想象
 * 以下标为行号，以值为列号，在二维数组里填充 1
 * 区间排名，即为选定若干连续的行，查询某列左边有几个 1
 * 区间第 k 小值，即为选定若干行，在这些行里从左往右找第 k 个 1
 * 修改某点点值，即将某个 1 不改变行号，改变列号
 * 所以本题可以用二维线段树解决
 */

using Tree = OY::VectorSumSegBIT<int>;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    const int N = 1e8;
    Tree tree(n, N + 1);
    for (int i = 0; i < n; i++) {
        tree.add(i, a[i], 1);
    }

    while (q--) {
        int op;
        std::cin >> op;

        if (op == 1) {
            int l, r, k;
            std::cin >> l >> r >> k;
            l--;
            std::cout << (k ? tree.query(l, r, 0, k) + 1 : 1) << '\n';
        } else if (op == 2) {
            int l, r, k;
            std::cin >> l >> r >> k;
            l--, k--;
            std::cout << tree.kth(l, r, k) << '\n';
        } else if (op == 3) {
            int pos, k;
            std::cin >> pos >> k;
            pos--;
            tree.add(pos, a[pos], -1);
            a[pos] = k;
            tree.add(pos, a[pos], 1);
        } else if (op == 4) {
            int l, r, k;
            std::cin >> l >> r >> k;
            l--;
            if (k == 0) {
                std::cout << "-2147483647\n";
            } else {
                int v = tree.query(l, r, 0, k);
                if (v == 0) {
                    std::cout << "-2147483647\n";
                } else {
                    std::cout << tree.kth(l, r, v - 1) << '\n';
                }
            }
        } else {
            int l, r, k;
            std::cin >> l >> r >> k;
            l--;
            if (k == N) {
                std::cout << "2147483647\n";
            } else {
                int v = tree.query(l, r, k + 1, N + 1);
                if (v == 0) {
                    std::cout << "2147483647\n";
                } else {
                    std::cout << tree.kth(l, r, tree.query(l, r, 0, k + 1)) << '\n';
                }
            }
        }
    }

    return 0;
}   
#include <bits/stdc++.h>
#include "src/DataStruct/container/MinMaxHeap.h"

/*
[Double-Ended Priority Queue](https://judge.yosupo.jp/problem/double_ended_priority_queue)(https://github.com/yosupo06/library-checker-problems/issues/874)
[status](https://judge.yosupo.jp/submission/318984)
*/
/*
给定一个多重集，每次操作：
1. 添加一个元素
2. 输出最小元素并删除
3. 输出最大元素并删除
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    auto read = [](auto...) {
        int x;
        std::cin >> x;
        return x;
    };
    OY::MMHeap::Heap<int> heap(n, read);

    while (q--) {
        int op;
        std::cin >> op;
        if (op == 0) {
            int x;
            std::cin >> x;
            heap.push(x);
        } else if (op == 1) {
            std::cout << heap.top_min() << "\n";
            heap.pop_min();
        } else if (op == 2) {
            std::cout << heap.top_max() << "\n";
            heap.pop_max();
        }
    }

    return 0;
}
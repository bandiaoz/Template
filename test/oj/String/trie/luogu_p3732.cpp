#include <bits/stdc++.h>
#include "src/String/trie/Trie.h"

/*
[P3732 [HAOI2017]供给侧改革](https://www.luogu.com.cn/problem/P3732)
[status](https://www.luogu.com.cn/record/237638122)
*/
/*
给定一个长度为 $$n(1 \leq n \leq 10^5)$$ 的01字符串，$$\mathrm{data}(l, r)$$ 表示起始位置从 $$[l, r]$$ 开始的后缀的 lcp

有 $$q(1 \leq q \leq 10^5)$$ 次询问，每次询问一个区间 $$[l, r]$$，求 $$\sum_{i = l}^{r - 1} \mathrm{data}(i, r)$$
01串随机生成

由于 01 串随机生成，故 lcp 不会很大，将询问离线下来，按照右端点排序
res[len] 表示长度为 len 的 lcp 的左端点，在插入时，如果当前节点有 id，则更新 res[len]
*/

template <typename Node>
struct NodeWrap : OY::Trie::BaseNodeWrap<Node> {
    int id = -1;
};

using Node = OY::StaticTrie<NodeWrap, 2>::node;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::string s;
    std::cin >> s;
    std::vector<std::array<int, 3>> queries(q);
    for (int i = 0; i < q; i++) {
        int l, r;
        std::cin >> l >> r;
        l--;
        queries[i] = {l, r, i};
    }
    std::sort(queries.begin(), queries.end(), [&](auto &lhs, auto &rhs) {
        return lhs[1] < rhs[1];
    });

    OY::StaticTrie<NodeWrap, 2> trie;
    const int LEN = 50;
    std::vector<int> ans(q);
    std::vector<int> res(LEN + 1, -1);
    for (int i = 0, j = 0; i < q; i++) {
        auto [l, r, id] = queries[i];
        while (j < r) {
            std::string suffix = s.substr(j, LEN);
            auto mapping = [&](int k) { return suffix[k] - '0'; };
            int len = 0;
            trie.insert(suffix.size(), mapping, [&](Node *node) {
                if (node == trie.get_node(0)) return;
                len++;
                if (node->id != -1) {
                    res[len] = std::max(res[len], node->id);
                }
                node->id = j;
            });
            j++;
        }
        int left = l;
        for (int len = LEN; len >= 1; len--) {
            if (res[len] >= left) {
                int cnt = res[len] - left + 1;
                ans[id] += len * cnt;
                left = res[len] + 1;
            }
        }
    }

    for (auto i : ans) {
        std::cout << i << '\n';
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/AssignZkwTree.h"

/*
[CF558E A Simple Task](https://codeforces.com/contest/558/problem/E)
[status](https://codeforces.com/contest/558/submission/338947819)
*/
/*
给定一个长度为 $$n$$ 的字符串 $$s$$ 和 $$q$$ 次操作，如果 $$op = 1$$，则将子串 $$[l, r]$$ 升序排序，如果 $$op = 0$$，则将子串 $$[l, r]$$ 降序排序，最后输出排序后的字符串。
$$1 \leq n \leq 10^5, 0 \leq q \leq 5 \times 10^4$$

开 26 棵区间推平线段树，维护每个字母的出现次数，排序时，做 26 次区间覆盖即可
*/

using Info = int;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::string s;
    std::cin >> s;

    std::vector<OY::AssignSumZkw<Info>> zkw(26);
    for (int i = 0; i < 26; i++) {
        zkw[i].resize(n, [&](int j) { return s[j] - 'a' == i; }, 0);
    }

    while (q--) {
        int l, r, op;
        std::cin >> l >> r >> op;
        l--;
        std::vector<int> cnt(26);
        for (int i = 0; i < 26; i++) {
            cnt[i] = zkw[i].query(l, r);
        }

        int begin = l;
        if (op == 1) {
            for (int i = 0; i < 26; i++) {
                zkw[i].modify(l, r, 0);
                if (cnt[i]) {
                    zkw[i].modify(begin, begin + cnt[i], 1);
                    begin += cnt[i];
                }
            }
        } else {
            for (int i = 25; i >= 0; i--) {
                zkw[i].modify(l, r, 0);
                if (cnt[i]) {
                    zkw[i].modify(begin, begin + cnt[i], 1);
                    begin += cnt[i];
                }
            }
        }
    }

    std::string ans;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 26; j++) {
            if (zkw[j].query(i)) {
                ans += char(j + 'a');
                break;
            }
        }
    }

    std::cout << ans << "\n";

    return 0;
}
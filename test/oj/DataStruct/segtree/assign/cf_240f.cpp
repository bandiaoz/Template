#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/AssignZkwTree.h"

/*
[CF240F TorCoder](https://codeforces.com/contest/240/problem/F)
[status](https://codeforces.com/contest/240/submission/339052620)
*/
/*
给定长度为 $$n(1 \leq n \leq 10^5)$$ 的字符串，有 $$m(1 \leq m \leq 10^5)$$ 次操作，每次将子串 $$[l, r]$$ 重排为字典序最小的回文串，如果无法重排则不操作。输出最终的字符串。

开 26 棵区间推平线段树，维护每个字母的出现次数，排序时，做 26 次区间覆盖即可
*/

using Info = int;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::freopen("input.txt", "r", stdin);
    std::freopen("output.txt", "w", stdout);

    int n, m;
    std::cin >> n >> m;
    std::string s;
    std::cin >> s;

    std::vector<OY::AssignSumZkw<Info>> zkw(26);
    for (int i = 0; i < 26; i++) {
        zkw[i].resize(n, [&](int j) { return s[j] - 'a' == i; }, 0);
    }

    while (m--) {
        int l, r;
        std::cin >> l >> r;
        l--;
        std::vector<int> cnt(26);
        for (int i = 0; i < 26; i++) {
            cnt[i] = zkw[i].query(l, r);
        }

        if (std::count_if(cnt.begin(), cnt.end(), [](int x) { return x % 2 == 1; }) <= 1) {
            int begin = l, end = r;
            int ch = -1;
            for (int i = 0; i < 26; i++) {
                zkw[i].modify(l, r, 0);
                if (int half = cnt[i] / 2; half) {
                    zkw[i].modify(begin, begin + half, 1);
                    zkw[i].modify(end - half, end, 1);
                    begin += half;
                    end -= half;
                }
                if (cnt[i] % 2 == 1) {
                    ch = i;
                }
            }
            if (begin != end) {
                zkw[ch].modify(begin, end, 1);
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
#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/ChthollyZkwTree.h"
#include "src/DataStruct/Segtree/ChthollySegTree.h"

/*
[P2787 语文1（chin1）- 理理思维](https://www.luogu.com.cn/problem/P2787)
[status](https://www.luogu.com.cn/record/204631041)
*/
/**
 * 对于一个长度为 n 的字符串，有 q 次操作
 * 1. 询问区间 [l, r] 中字母 c 出现了多少次
 * 2. 将区间 [l, r] 的字母全部修改为 c
 * 3. 排序区间 [l, r]
 * 
 * 对 Segment 进行重写，维护区间内每个字母的出现次数
 * 除了 BaseSegment 的所有方法以外，还需要重载 `operator*`，
 * 操作 3 可以看成 1 和 2 的结合，先查询出所有字母的出现次数，再对每个字母进行区间赋值
 */

struct Segment {
    using value_type = char;
    uint16_t m_cnt[26]{};
    Segment() = default;
    Segment(char color) { m_cnt[color - 'a']++; }
    Segment operator+(const Segment &rhs) const {
        Segment res;
        for (uint32_t i = 0; i != 26; i++) {
            res.m_cnt[i] = m_cnt[i] + rhs.m_cnt[i];
        }
        return res;
    }
    Segment operator*(uint32_t n) const {
        Segment res;
        for (uint32_t i = 0; i != 26; i++) {
            res.m_cnt[i] = m_cnt[i] * n;
        }
        return res;
    }
    bool has_change() const {
        return std::count_if(m_cnt, m_cnt + 26, [](auto v) { return v != 0; }) > 1;
    }
    char get() const {
        return 'a' + (std::find_if(m_cnt, m_cnt + 26, [](auto v) { return v != 0; }) - m_cnt);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::string s;
    std::cin >> s;

    // OY::ChthollyFastPowZkwTree<Segment> cho(n, 'a');
    OY::VectorChthollyFastPowSegTree<Segment> cho(n, 'a');
    for (int i = 0; i < n; i++) {
        if (std::isupper(s[i])) {
            s[i] = std::tolower(s[i]);
        }
        if (s[i] != 'a') {
            cho.modify(i, s[i]);
        }
    }

    while (q--) {
        int op, l, r;
        std::cin >> op >> l >> r;
        l--;

        if (op == 1) {
            char c;
            std::cin >> c;
            if (std::isupper(c)) {
                c = std::tolower(c);
            }
            std::cout << cho.query(l, r).m_cnt[c - 'a'] << "\n";
        } else if (op == 2) {
            char c;
            std::cin >> c;
            if (std::isupper(c)) {
                c = std::tolower(c);
            }
            cho.modify(l, r, c);
        } else {
            auto res = cho.query(l, r);
            for (int i = 0, cur = l; i < 26; i++) {
                if (res.m_cnt[i]) {
                    cho.modify(cur, cur + res.m_cnt[i], 'a' + i);
                    cur += res.m_cnt[i];
                }
            }
        }
    }

    return 0;
}
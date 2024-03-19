#include <bits/stdc++.h>

using ll = long long;

std::vector<int> prefixFunction(std::string s) {
    int n = (int)s.size();
    std::vector<int> p(n);
    for (int i = 1; i < n; ++i) {
        int j = p[i - 1];
        while (j > 0 && s[i] != s[j]) j = p[j - 1];
        if (s[i] == s[j]) ++j;
        p[i] = j;
    }
    return p;
}

// KMP based on prefixFunction. return all match postion in t
// also can create string st = s + '#' + t, and call prefixFunction(st),
// if p[i] == s.length()，it's a successful match: s in t
std::vector<int> kmp(std::string s, std::string t) {
    std::vector<int> ans;
    int n = (int)s.size(), m = (int)t.size();
    if (n > m) return ans;
    auto p = prefixFunction(s);
    for (int i = 0, j = 0; i < m; ++i) {
        while (j > 0 && s[j] != t[i]) j = p[j - 1];
        if (s[j] == t[i] && ++j == n) ans.emplace_back(i - n + 1);
    }
    return ans;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string t, s;
    std::cin >> t >> s;

    std::string st = s + '#' + t;
    auto ans = prefixFunction(st);
    for (int i = s.length() + 1; i < st.length(); ++i) {
        if (ans[i] == s.length()) {
            std::cout << i - 2 * s.length() + 1 << "\n";
        }
    }

    for (int i = 0; i < s.length(); ++i) {
        std::cout << ans[i] << " \n"[i == s.length() - 1];
    }

    return 0;
}

// test problem: https://www.luogu.com.cn/problem/P3375
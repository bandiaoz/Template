#include <bits/stdc++.h>

/**
 * @brief 计算前缀函数
 * @note 返回数组p，p[i]表示s[0:i]的最长前缀后缀长度
 */
std::vector<int> prefixFunction(std::string s) {
    int n = (int)s.size();
    std::vector<int> p(n);
    for (int i = 1; i < n; i++) {
        int j = p[i - 1];
        while (j > 0 && s[i] != s[j]) j = p[j - 1];
        if (s[i] == s[j]) ++j;
        p[i] = j;
    }
    return p;
}

/**
 * @brief KMP based on prefixFunction. 查找模式串 s 在文本串 t 中所有出现的位置
 * @param s string 模式串
 * @param t string 文本串
 * @return return all match postion in t
 * @note also can create string st = s + '#' + t, and call prefixFunction(st), if p[i] == s.length()，it's a successful match: s in t
 * @link https://www.luogu.com.cn/problem/P3375
*/
std::vector<int> kmp(std::string s, std::string t) {
    std::vector<int> ans;
    int n = (int)s.size(), m = (int)t.size();
    if (n > m) return ans;
    auto p = prefixFunction(s);
    for (int i = 0, j = 0; i < m; i++) {
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

    if (true) { 
        // method 1: st = s + '#' + t, and call prefixFunction(st)
        std::string st = s + '#' + t;
        auto p = prefixFunction(st);
        for (int i = s.length() + 1; i < st.length(); i++) {
            if (p[i] == s.length()) {
                std::cout << i - 2 * s.length() + 1 << "\n";
            }
        }

        for (int i = 0; i < s.length(); i++) {
            std::cout << p[i] << " \n"[i == s.length() - 1];
        }
    } else { 
        // method 2: call kmp(s, t)
        auto ans = kmp(s, t);
        for (int i = 0; i < ans.size(); i++) {
            std::cout << ans[i] + 1 << "\n";
        }

        auto p = prefixFunction(s);
        for (int i = 0; i < p.size(); i++) {
            std::cout << p[i] << " \n"[i == p.size() - 1];
        }
    }

    return 0;
}

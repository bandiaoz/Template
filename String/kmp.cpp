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
 * @param s string 模式串，长度为 n
 * @param t string 文本串，长度为 m
 * @return ans[i] 表示 s[0:n] 在 t[ans[i]:ans[i]+n] 中出现
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
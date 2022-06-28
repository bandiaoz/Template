#include <bits/stdc++.h>

using namespace std;
using ll = long long;

vector<int> prefixFunction(string s) {
    int n = (int)s.size();
    vector<int> p(n);
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
vector<int> kmp(string s, string t) {
    vector<int> ans;
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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, t;
    cin >> s >> t;

    auto ans = kmp(t, s);
    for (int i = 0; i < ans.size(); ++i) {
        cout << ans[i] + 1 << "\n";
    }

    auto p = prefixFunction(t);
    for (int i = 0; i < p.size(); ++i) {
        cout << p[i] << " \n"[i == p.size() - 1];
    }

    return 0;
}

// test problem: https://www.luogu.com.cn/problem/P3375
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

class Trie {
    vector<vector<int>> ch_;
    vector<int> cnt;
    int getnum(char ch) {
        if (ch <= '9') return ch - '0';
        if (ch <= 'Z') return ch - 'A' + 10;
        return ch - 'a' + 36;
    }
    int emptyNode() {
        ch_.push_back(vector<int>(62, -1));
        cnt.push_back(0);
        return ch_.size() - 1;
    }

   public:
    Trie() { emptyNode(); }
    void insert(string s) {
        for (int i = 0, p = 0; i < s.size(); ++i) {
            int c = getnum(s[i]);
            if (ch_[p][c] == -1) {
                ch_[p][c] = emptyNode();
            }
            p = ch_[p][c];
            cnt[p]++;
        }
    }
    int query(string s) {
        int p = 0;
        for (int i = 0; i < s.size(); ++i) {
            int c = getnum(s[i]);
            if (ch_[p][c] == -1) return 0;
            p = ch_[p][c];
        }
        return cnt[p];
    }
};

void solve() {
    int n, q;
    cin >> n >> q;

    Trie trie;
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        trie.insert(s);
    }

    while (q--) {
        string s;
        cin >> s;
        cout << trie.query(s) << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
// test problem: https://www.luogu.com.cn/problem/P8306
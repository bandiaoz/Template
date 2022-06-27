#include <bits/stdc++.h>

using namespace std;
using ll = long long;

class Automaton {
    static inline constexpr int CHAR = 26;
    using Node = array<int, CHAR>;
    vector<Node> nxt_;
    vector<int> cnt_, fail_, last_;
    int charToInt(char x) { return x - 'a'; }
    void addNode(int fa, int c) {
        nxt_[fa][c] = nxt_.size();
        nxt_.emplace_back(Node());
        cnt_.emplace_back(0);
        fail_.emplace_back(0);
        last_.emplace_back(0);
    }

   public:
    Automaton() : nxt_(1), cnt_(1), fail_(1), last_(1) {}
    void insert(string s) {
        int p = 0;
        for (auto x : s) {
            int c = charToInt(x);
            if (nxt_[p][c] == 0) addNode(p, c);
            p = nxt_[p][c];
        }
        ++cnt_[p];
    }
    void build() {
        queue<int> Q;
        for (int c = 0; c < CHAR; ++c) {
            if (nxt_[0][c]) Q.push(nxt_[0][c]);
        }
        while (!Q.empty()) {
            int p = Q.front();
            Q.pop();
            for (int c = 0; c < CHAR; ++c) {
                if (int& q = nxt_[p][c]; q != 0) {
                    fail_[q] = nxt_[fail_[p]][c];
                    Q.push(q);
                    // match count optim
                    last_[q] = cnt_[fail_[q]] ? fail_[q] : last_[fail_[q]];
                } else {
                    q = nxt_[fail_[p]][c];
                }
            }
        }
    }
    // case-by-case analysis
    int query(string s) {
        int p = 0, r = 0;
        auto add = [&](int& x) {
            r += x;
            x = 0;
        };
        for (auto x : s) {
            int c = charToInt(x);
            p = nxt_[p][c];
            if (cnt_[p]) add(cnt_[p]);
            int q = p;
            while (last_[q]) {
                q = last_[q];
                if (cnt_[q]) add(cnt_[q]);
            }
        }
        return r;
    }
};
// https://www.luogu.com.cn/problem/P3808

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;

    Automaton ac;
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        ac.insert(s);
    }

    ac.build();

    string s;
    cin >> s;

    cout << ac.query(s) << "\n";

    return 0;
}
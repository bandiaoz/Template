#include <bits/stdc++.h>

using namespace std;
using ll = long long;

/** Modified from:
 * https://github.com/kth-competitive-programming/kactl/blob/master/content/strings/AhoCorasick.h
 * Try to handle duplicated patterns beforehand, otherwise change 'end' to
 * vector; empty patterns are not allowed. Time: construction takes $O(26N)$,
 * where $N =$ sum of length of patterns. find(x) is $O(N)$, where N = length of x. 
 * findAll is $O(N+M)$ where M is number of occurrence of all pattern (up to N*sqrt(N)) */
struct AhoCorasick {
    enum { alpha = 26, first = 'a' }; // change this!
    struct Node {
        // back: failure link, points to longest suffix that is in the trie.
        // end: longest pattern that ends here, is -1 if no patten ends here.
        // nmatches: number of (patterns that is a suffix of current node)/(duplicated patterns), depends on needs.
        // output: output link, points to the longest pattern that is a suffix of current node
        int back, end = -1, nmatches = 0, output = -1;
        array<int, alpha> ch;
        Node(int v = -1) { fill(ch.begin(), ch.end(), v); }
    };
    vector<Node> N;
    int n;
    AhoCorasick() : N(1), n(0) {}
    void insert(string &s) {
        assert(!s.empty());
        int p = 0;
        for (char c : s) {
            if (N[p].ch[c - first] == -1) {
                N[p].ch[c - first] = N.size();
                N.emplace_back();
            }
            p = N[p].ch[c - first];
        }
        N[p].end = n++;
        N[p].nmatches++;
    }
    void build() {
        N[0].back = (int)N.size();
        N.emplace_back(0);
        queue<int> q;
        q.push(0);
        while (!q.empty()) {
            int p = q.front();
            q.pop();
            for (int i = 0; i < alpha; i++) {
                int pnx = N[N[p].back].ch[i];
                auto &nxt = N[N[p].ch[i]];
                if (N[p].ch[i] == -1) N[p].ch[i] = pnx;
                else {
                    nxt.back = pnx;
                    // if prev is an end node, then set output to prev node,
                    // otherwise set to output link of prev node
                    nxt.output = N[pnx].end == -1 ? N[pnx].output : pnx;
                    // if we don't want to distinguish info of patterns that is
                    // a suffix of current node, we can add info to the ch
                    // node like this: nxt.nmatches+=N[pnx].nmatches;
                    q.push(N[p].ch[i]);
                }
            }
        }
    }
    // for each position, finds the longest pattern that ends here
    vector<int> find(const string &text) {
        int len = text.length();
        vector<int> res(len);
        int p = 0;
        for (int i = 0; i < len; i++) {
            p = N[p].ch[text[i] - first];
            res[i] = N[p].end;
        }
        return res;
    }
    // for each position, finds the all that ends here
    vector<vector<int>> find_all(const string &text) {
        int len = text.length();
        vector<vector<int>> res(len);
        int p = 0;
        for (int i = 0; i < len; i++) {
            p = N[p].ch[text[i] - first];
            res[i].push_back(N[p].end);
            for (int ind = N[p].output; ind != -1; ind = N[ind].output) {
                assert(N[ind].end != -1);
                res[i].push_back(N[ind].end);
            }
        }
        return res;
    }
    int find_cnt(const string &text) {
        int len = text.length();
        vector<int> num(n + 1, 0);
        int p = 0, ans = 0;
        for (int i = 0; i < len; i++) {
            p = N[p].ch[text[i] - first];
            if (N[p].end != -1) {
                if (!num[N[p].end]) {
                    num[N[p].end]++;
                    ans += N[p].nmatches;
                }
            }
            for (int ind = N[p].output; ind != -1; ind = N[ind].output) {
                if (!num[N[ind].end]) {
                    num[N[ind].end]++;
                    ans += N[ind].nmatches;
                }
            }
        }
        return ans;
    }
    pair<int, vector<int>> find_maxcnt(const string &text) {
        int len = text.length();
        vector<int> num(n + 1, 0);
        int p = 0, ans = 0;
        for (int i = 0; i < len; i++) {
            p = N[p].ch[text[i] - first];
            if (N[p].end != -1) {
                if (!num[N[p].end]) {
                    num[N[p].end]++;
                    ans = max(ans, N[p].nmatches);
                }
            }
            for (int ind = N[p].output; ind != -1; ind = N[ind].output) {
                if (!num[N[ind].end]) {
                    num[N[ind].end]++;
                    ans += N[ind].nmatches;
                }
            }
        }
        vector<int> idx;
        for (int i = 0; i < n; i++) {
            if (num[i] == ans) {
                idx.push_back(i);
            }
        }
        return pair(ans, idx);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    AhoCorasick ac;
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        ac.insert(s);
    }

    ac.build();

    string t;
    cin >> t;

    cout << ac.find_cnt(t) << "\n";

    return 0;
}

// test problem: https://www.luogu.com.cn/problem/P3808
#include <cstring>
#include <iostream>
#include <queue>
using namespace std;
const int N = 5e5 + 10;
struct Trie {
    static const char stdc = 'a';
    static const int M = 26;
    int ch[N][M];
    int val[N];
    // int num[N];
    int root;
    int fail[N];
    int sz;
    int idx(char c) {
        return c - stdc;
    }
    void init() {
        root = 0;
        sz = 1;
        memset(val, 0, sizeof val);
        // memset(num, 0, sizeof num);
        memset(ch[0], 0, sizeof ch[0]);
    }

    void insert(string &str) {
        int len = str.size();
        int s = 0;
        for (int i = 0; i < len; ++i) {
            int u = idx(str[i]);
            if (!ch[s][u]) {
                memset(ch[sz], 0, sizeof ch[sz]);
                ch[s][u] = sz++;
            }
            s = ch[s][u];
        }
        val[s]++;
    }
    void build() {
        queue<int> Q;
        fail[0] = 0;
        for (int i = 0; i < M; ++i) {
            int u = ch[0][i];
            if (u) {
                fail[u] = 0;
                Q.push(u);
            }
        }
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();
            for (int i = 0; i < M; ++i) {
                int r = ch[u][i];
                if (!r) {
                    continue;
                }
                Q.push(r);
                int v = fail[u];
                while (v && !ch[v][i])
                    v = fail[v];
                fail[r] = ch[v][i];
            }
        }
    }
    int query(string &str) {
        int s = root, ans = 0;
        int len = str.size();
        for (int i = 0; i < len; ++i) {
            int id = idx(str[i]);
            while (s && ch[s][id] == 0) {
                s = fail[s];
            }
            s = ch[s][id];
            int u = s;
            while (u && val[u] != -1) {
                ans += val[u];
                val[u] = -1;
                u = fail[u];
            }
        }
        return ans;
    }
    void debug() {}
} ac;
int main() {}
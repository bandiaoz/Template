#include <bits/stdc++.h>
#define sci(n) scanf("%d", &n)
#define scl(n) scanf("%lld", &n)
using namespace std;
typedef long long ll;

const int INF = 0x7fffffff;
const int N = 100005;
int ch[N][2];
int val[N];
int cnt[N];
int par[N];
int sz[N];
int root;
int ncnt;

bool chk(int x) {
    return ch[par[x]][1] == x;
}

void pushup(int x) {
    sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + cnt[x];
}

void rotate(int x) {
    int y = par[x], z = par[y], k = chk(x), w = ch[x][k ^ 1];
    ch[y][k] = w;
    par[w] = y;
    ch[z][chk(y)] = x;
    par[x] = z;
    ch[x][k ^ 1] = y;
    par[y] = x;
    pushup(y);
    pushup(x);
}

void splay(int x, int goal = 0) {
    while (par[x] != goal) {
        int y = par[x], z = par[y];
        if (z != goal) {
            if (chk(x) == chk(y))
                rotate(y);
            else
                rotate(x);
        }
        rotate(x);
    }
    if (!goal)
        root = x;
}

void find(int x) {
    if (!root)
        return;
    int cur = root;
    while (ch[cur][x > val[cur]] && x != val[cur])
        cur = ch[cur][x > val[cur]];
    splay(cur);
}

void insert(int x) {
    int cur = root, p = 0;
    while (cur && val[cur] != x) {
        p = cur;
        cur = ch[cur][x > val[cur]];
    }
    if (cur)
        cnt[cur]++;
    else {
        cur = ++ncnt;
        if (p)
            ch[p][x > val[p]] = cur;
        ch[cur][0] = ch[cur][1] = 0;
        val[cur] = x;
        par[cur] = p;
        cnt[cur] = sz[cur] = 1;
    }
    splay(cur);
}

int kth(int k) {
    int cur = root;
    while (1) {
        if (ch[cur][0] && k <= sz[ch[cur][0]])
            cur = ch[cur][0];
        else if (k > sz[ch[cur][0]] + cnt[cur]) {
            k -= sz[ch[cur][0]] + cnt[cur];
            cur = ch[cur][1];
        } else
            return cur;
    }
}

int getrank(int x) {
    find(x);
    return sz[ch[root][0]];
}

int pre(int x) {
    find(x);
    if (val[root] < x)
        return root;
    int cur = ch[root][0];
    while (ch[cur][1])
        cur = ch[cur][1];
    return cur;
}

int succ(int x) {
    find(x);
    if (val[root] > x)
        return root;
    int cur = ch[root][1];
    while (ch[cur][0])
        cur = ch[cur][0];
    return cur;
}

void remove(int x) {
    int last = pre(x), next = succ(x);
    splay(last);
    splay(next, last);
    int del = ch[next][0];
    if (cnt[del] > 1) {
        cnt[del]--;
        splay(del);
    } else
        ch[next][0] = 0;
}

int main(void) {
    int n, opt, x;
    scanf("%d", &n);
    insert(INF);
    insert(-INF);
    while (n--) {
        scanf("%d%d", &opt, &x);
        if (opt == 1)
            insert(x);
        else if (opt == 2)
            remove(x);
        else if (opt == 3)
            printf("%d\n", getrank(x));
        else if (opt == 4)
            printf("%d\n", val[kth(x + 1)]);
        else if (opt == 5)
            printf("%d\n", val[pre(x)]);
        else if (opt == 6)
            printf("%d\n", val[succ(x)]);
    }
}
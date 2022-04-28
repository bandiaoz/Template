#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
#define IO ios::sync_with_stdio(false), cin.tie(0), cout.tie(0)
typedef long long ll;
const int maxm = 1e4 + 5;
const int maxn = 105;
int k;
int f[maxn], sg[maxm], vis[maxm];  //记录后继状态
void dosg() {
    sg[0] = 0;
    memset(vis, -1, sizeof(vis));
    for (int i = 1; i < maxm; ++i) {
        for (int j = 1; f[j] <= i && j <= k; ++j)
            vis[sg[i - f[j]]] = i;
        int j = 0;
        while (vis[j] == i)
            ++j;
        sg[i] = j;
    }
}

int main() {
    while (scanf("%d", &k) && k) {
        for (int i = 1; i <= k; ++i)
            scanf("%d", &f[i]);
        sort(f + 1, f + 1 + k);
        dosg();
        int m;
        scanf("%d", &m);
        while (m--) {
            int n;
            scanf("%d", &n);
            int ans = 0;
            for (int i = 1; i <= n; ++i) {
                int x;
                scanf("%d", &x);
                ans ^= sg[x];
            }
            printf("%c", ans ? 'W' : 'L');
            // cout << (ans ? "W" : "L");
        }
        printf("\n");
    }
}
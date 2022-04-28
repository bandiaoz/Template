#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
using namespace std;
typedef long long ll;
int a, b, num[20], dp[20][2];
int dfs(int len, bool if6, bool lim) {
    if (len == 0) return 1;
    if (!lim && dp[len][if6]) return dp[len][if6];
    int cnt = 0, maxx = (lim ? num[len] : 9);
    for (int i = 0; i <= maxx; i++) {
        if (i == 4 || (if6 && i == 2)) continue;
        cnt += dfs(len - 1, i == 6, lim && i == maxx);
    }
    return lim ? cnt : dp[len][if6] = cnt;
}

int solve(int x) {
    memset(num, 0, sizeof(num));
    int k = 0;
    while (x) {
        num[++k] = x % 10;
        x /= 10;
    }
    return dfs(k, false, true);
}

int main() {
    scanf("%d%d", &a, &b);
    printf("%d\n", solve(b) - solve(a - 1));
}
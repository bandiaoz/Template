#include <cmath>
#include <cstring>
#include <iostream>
#define gold (sqrt(5.0) + 1) / 2
using namespace std;
typedef long long ll;

int sg[1005];
const int N = 30;
int f[N];
int s[1005];
void DoSg(int num) {
    int i, j;
    memset(sg, 0, sizeof(sg));
    for (i = 1; i <= num; ++i) {
        memset(s, 0, sizeof(s));
        for (j = 0; f[j] <= i && j < N; ++j) {
            s[sg[i - f[j]]] = 1;
        }
        for (j = 0;; ++j) {
            if (!s[j]) {
                sg[i] = j;
                break;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    f[0] = 1;
    f[1] = 1;
    for (int i = 2; i <= 30; ++i) {
        f[i] = f[i - 1] + f[i - 2];
    }
    DoSg(1000);
    int n, m, k;
    while (cin >> n >> m >> k) {
        if (n == 0 && m == 0 && k == 0)
            break;
        if (sg[n] ^ sg[m] ^ sg[k])
            cout << "Fibo" << endl;
        else
            cout << "Nqcci" << endl;
    }
}
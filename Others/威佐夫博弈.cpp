#include <algorithm>
#include <cmath>
#include <iostream>
#define gold (sqrt(5.0) + 1) / 2
using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    int a, b;
    while (cin >> a >> b) {
        int big = max(a, b);
        int small = min(a, b);
        double now = double(big - small) * gold;
        if ((int)now == small)
            cout << 0 << endl;  //后手必胜
        else
            cout << 1 << endl;  //先手必胜
    }
}
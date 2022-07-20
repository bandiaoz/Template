#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template <typename T>
T floor(T a, T n) {
    if (n < 0) {
        n = -n;
        a = -a;
    }
    return a < 0 ? (a - n + 1) / n : a / n;
}

template <typename T>
T ceil(T a, T n) {
    if (n < 0) {
        n = -n;
        a = -a;
    }
    return a < 0 ? a / n : (a + n - 1) / n;
}

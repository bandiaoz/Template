#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template <typename T>
vector<int> manacher(int n, const T &s) {
    if (n == 0) {
        return vector<int>();
    }
    vector<int> res(2 * n - 1, 0);
    int l = -1, r = -1;
    for (int z = 0; z < 2 * n - 1; z++) {
        int i = (z + 1) >> 1;
        int j = z >> 1;
        int p = (i >= r ? 0 : min(r - i, res[2 * (l + r) - z]));
        while (j + p + 1 < n && i - p - 1 >= 0) {
            if (!(s[j + p + 1] == s[i - p - 1])) {
                break;
            }
            p++;
        }
        if (j + p > r) {
            l = i - p;
            r = j + p;
        }
        res[z] = p;
    }
    return res;
    // res[2 * i] = odd radius in position i
    // res[2 * i + 1] = even radius between positions i and i + 1
    // s = "abaa" -> res = {0, 0, 1, 0, 0, 1, 0}
    // s = "aaa" -> res = {0, 1, 1, 1, 0}
    // in other words, for every z from 0 to 2 * n - 2:
    // calculate i = (z + 1) >> 1 and j = z >> 1
    // now there is a palindrome from i - res[z] to j + res[z]
    // (watch out for i > j and res[z] = 0)
}
template <typename T>
vector<int> manacher(const T &s) {
    return manacher((int)s.size(), s);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    int n = s.length();

    auto ans = manacher(s);

    int len = 0, id = -1;
    for (int z = 0; z < 2 * n - 1; ++z) {
        if (z % 2 == 0 && 1 + 2 * ans[z] > len) { // odd length of palindrome
            len = 1 + 2 * ans[z];
            id = z / 2 - ans[z];
        } else if (z % 2 == 1 && 2 * ans[z] > len) { // even length of palindrome
            len = 2 * ans[z];
            id = z / 2 - ans[z] + 1;
        }
    }

    cout << s.substr(id, len) << "\n";

    return 0;
}
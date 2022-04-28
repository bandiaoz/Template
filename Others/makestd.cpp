#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    while (getline(cin, s)) {
        cout << "\"";
        for (auto it : s) {
            if (it == '"' || it == '\\')
                cout << "\\";
            cout << it;
        }
        cout << "\",";
        cout << endl;
    }
    return 0;
}
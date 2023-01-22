#include <bits/stdc++.h>
#include <bits/extc++.h>

using namespace std;
using namespace __gnu_pbds;
using ll = long long;

const int RANDOM = chrono::high_resolution_clock::now().time_since_epoch().count();
struct chash {
    int operator()(int x) const { 
        return x ^ RANDOM; 
    }
};
struct chash {
    int operator()(pair<int, int> x) const { 
        return x.first * 31 + x.second; 
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    gp_hash_table<ll, int> mp;

    gp_hash_table<ll, int, chash> table;
    
    

    return 0;
}
#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt32.h"
#include "src/Math/LinearAlgebra/HamelXorBase.h"

using ll = long long;
using Z = OY::mint1000000007;

/*
[CF895C Square Subsets](https://codeforces.com/contest/895/problem/C)
[status](https://codeforces.com/contest/895/submission/310998977)
*/
/**
 * 给定一个数组，求有多少个非空子集，使得子集内所有数的乘积是一个完全平方数。
 * 
 * 考虑质因数分解，如果一个数是完全平方数，那么每个质因数的幂次都是偶数。
 * 将质因子用二进制进行压缩，插入到线性基中，如果基向量数量 s 小于 n，则答案为 2^(n-s) - 1
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n; 

    std::map<int, int> id;
    auto is_prime = [](int x) {
        for (int i = 2; i * i <= x; i++) {
            if (x % i == 0) return false;
        }
        return true;
    };
    for (int i = 2; i <= 70; i++) {
        if (is_prime(i)) {
            id[i] = id.size();
        }
    }

    auto get = [&](int x) {
        ll ans = 0;
        for (int p = 2; p * p <= x; p++) {
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                cnt++;
            }
            if (cnt % 2 == 1) {
                ans |= 1LL << id[p];
            }
        }
        if (x > 1) {
            ans |= 1LL << id[x];
        }
        return ans;
    };
    
    OY::StaticHamelXorBase64<64> hxb;
    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        hxb.insert(get(x));
    }

    if (hxb.base_count() != n) {
        std::cout << Z(2).pow(n - hxb.base_count()) - 1 << "\n";
    } else {
        std::cout << "0\n";
    }

    return 0;
}
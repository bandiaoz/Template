#include <bits/stdc++.h>

struct EulerSieveSimple {
    const int N;
    std::vector<int> minp, primes;

    EulerSieveSimple(int n) : N(n), minp(n + 1) {
        for (int i = 2; i <= N; ++i) {
            if (!minp[i]) {
                minp[i] = i;
                primes.push_back(i);
            }
            for (auto p : primes) {
                if (i * p > n) break;
                minp[i * p] = p;
                if (i % p == 0) break;
            }
        }
    }
    std::vector<std::pair<int, int>> prime_factors(int n) {
        std::vector<std::pair<int, int>> factors;
        while (n > 1) {
            int p = minp[n], cnt = 0;
            while (n % p == 0) {
                cnt++;
                n /= p;
            }
            factors.emplace_back(p, cnt);
        }
        return factors;
    };
    std::vector<int> factors(int n) {
        std::vector<int> ans{1};
        while (n != 1) {
            int p = minp[n], cnt = 0;
            while (n % p == 0) {
                cnt++;
                n /= p;
            }
            int l = ans.size();
            for (int i = 0; i < l; i++) {
                for (int j = 0, t = p; j < cnt; j++, t *= p) {
                    ans.push_back(ans[i] * t);
                }
            }
        }
        return ans;
    };
};

#include <bits/stdc++.h>

struct EluerSieveSimple {
    const int N;
    std::vector<int> minp, primes;

    EluerSieveSimple(int n) : N(n), minp(n + 1) {
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
    std::vector<std::pair<int, int>> factor(int n) {
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
};

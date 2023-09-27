#include <bits/stdc++.h>

struct EulerSieve {
    const int N;
    std::vector<int> minp, num, d, phi, primes;

    // minp[i] is the minimum prime factor of i
    // d[i] is the number of factors of i
    // num[i] is the number of smallest prime factors of i
    EulerSieve(int n) : N(n), minp(n + 1), num(n + 1), d(n + 1), phi(n + 1) {
        phi[1] = 1;
        d[1] = 1;
        for (int i = 2; i <= N; ++i) {
            if (!minp[i]) {
                minp[i] = i;
                num[i] = 1;
                d[i] = 2;
                phi[i] = i - 1;
                primes.push_back(i);
            }
            for (auto p : primes) {
                if (i * p > n) break;

                minp[i * p] = p;
                if (i % p == 0) {
                    num[i * p] = num[i] + 1;
                    d[i * p] = d[i] / num[i * p] * (num[i * p] + 1);
                    phi[i * p] = phi[i] * p;
                    break;
                } else {
                    num[i * p] = 1;
                    d[i * p] = d[i] * 2;
                    phi[i * p] = phi[i] * phi[p];
                }
            }
        }
    }
    int euler_phi(int n) {
        int ans = n;
        for (int i = 2; i * i <= n; i++)
            if (n % i == 0) {
                ans = ans / i * (i - 1);
                while (n % i == 0) n /= i;
            }
        if (n > 1) ans = ans / n * (n - 1);
        return ans;
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

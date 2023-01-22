#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct EluerSieve {
    const int N;
    vector<int> mp, num, d, phi, primes;

    // mp[i] is the minimum prime factor of i
    // d[i] is the number of factors of i
    // num[i] is the number of minimun prime factors of i
    EluerSieve(int n) : N(n), mp(n + 1), num(n + 1), d(n + 1), phi(n + 1) {
        phi[1] = 1;
        d[1] = 1;
        for (int i = 2; i <= N; ++i) {
            if (!mp[i]) {
                mp[i] = i;
                num[i] = 1;
                d[i] = 2;
                phi[i] = i - 1;
                primes.push_back(i);
            }

            for (int j = 0; j < primes.size() && i * primes[j] <= N; ++j) {
                int p = primes[j];
                mp[i * p] = p;

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
};

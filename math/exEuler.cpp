#include <bits/stdc++.h>

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

int power(int a, int b, int m) {
    int ans = 1;
    while (b) {
        if (b & 1) ans = 1LL * ans * a % m;
        a = 1LL * a * a % m;
        b >>= 1;
    }
    return ans;
}

/**
 * @brief Calculate a^b mod m using Extend Euler's theorem
 * @param a int
 * @param b string, b is a string because b may be very large
 * @param m int
 * @return a^b mod m
 * @note a^{b} \equiv \begin{cases}
            a^b & , b < \varphi(m) \\
            a^{b \bmod \varphi(m) + \varphi(m)} & , b \geq \varphi(m)
         \end{cases} (\bmod m)
*/
int euler(int a, std::string b, int m) {
    a %= m;
    int phi = euler_phi(m);
    int num_b = 0;
    for (char c : b) {
        num_b = num_b * 10 + c - '0';
        if (num_b >= phi) {
            num_b = num_b % phi + phi;
        }
    }
    return power(a, num_b, m);
}
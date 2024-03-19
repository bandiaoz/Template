#pragma once

#include "../Others/Z.cpp"
#include <chrono>
#include <random>
#include <string>
#include <vector>

struct string_hash {
    const int n;
    const int64_t Base1 = 457, MOD1 = 1e9 + 7;
    const int64_t Base2 = 131, MOD2 = 998244353;
    int64_t *ha1, *ha2, *pow1, *pow2;
    int64_t *rha1, *rha2;
    string_hash(const std::string &ss, int n1) : n(n1) {
        ha1 = new int64_t[n + 1];
        ha2 = new int64_t[n + 1];
        pow1 = new int64_t[n + 1];
        pow2 = new int64_t[n + 1];
        rha1 = new int64_t[n + 1];
        rha2 = new int64_t[n + 1];
        pow1[0] = pow2[0] = 1;
        for (int i = 1; i <= n; i++) {
            pow1[i] = pow1[i - 1] * Base1 % MOD1;
            pow2[i] = pow2[i - 1] * Base2 % MOD2;
        }
        ha1[0] = ha2[0] = rha1[0] = rha2[0] = 0;
        for (int i = 1; i <= n; i++) {
            ha1[i] = (ha1[i - 1] * Base1 + ss[i - 1]) % MOD1;
            ha2[i] = (ha2[i - 1] * Base2 + ss[i - 1]) % MOD2;
            rha1[i] = (rha1[i - 1] * Base1 + ss[n - i]) % MOD1;
            rha2[i] = (rha2[i - 1] * Base2 + ss[n - i]) % MOD2;
        }
    }
    std::pair<int64_t, int64_t> get_hash(int l, int r) {
        int64_t res1 =
            ((ha1[r] - ha1[l - 1] * pow1[r - l + 1]) % MOD1 + MOD1) % MOD1;
        int64_t res2 =
            ((ha2[r] - ha2[l - 1] * pow2[r - l + 1]) % MOD2 + MOD2) % MOD2;
        return {res1, res2};
    }
    std::pair<int64_t, int64_t> get_rhash(int l, int r) {
        int64_t res1 = ((rha1[n - l + 1] - rha1[n - r] * pow1[r - l + 1]) % MOD1 + MOD1) % MOD1;
        int64_t res2 = ((rha2[n - l + 1] - rha2[n - r] * pow2[r - l + 1]) % MOD2 + MOD2) % MOD2;
        return {res1, res2};
    }
    bool is_palindrome(int l, int r) {  // 判断 ss[l, r] 是否为回文串
        return get_hash(l, r) == get_rhash(l, r);
    }
    std::pair<int64_t, int64_t> add(std::pair<int64_t, int64_t> aa,
                                    std::pair<int64_t, int64_t> bb) {
        int64_t res1 = (aa.first + bb.first) % MOD1;
        int64_t res2 = (aa.second + bb.second) % MOD2;
        return {res1, res2};
    }
    std::pair<int64_t, int64_t> mul(std::pair<int64_t, int64_t> aa,
                                    int64_t kk) {  // aa *= Base 的 k 次方
        int64_t res1 = aa.first * pow1[kk] % MOD1;
        int64_t res2 = aa.second * pow2[kk] % MOD2;
        return {res1, res2};
    }
    std::pair<int64_t, int64_t> pin(int l1, int r1, int l2, int r2) {  // 拼接字符串 r1 < l2  ss = s1 + s2
        return add(get_hash(l2, r2), mul(get_hash(l1, r1), r2 - l2 + 1));
    }
};
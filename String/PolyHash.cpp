#include <bits/stdc++.h>

struct PolyHash {
    static constexpr int mod = 1e9 + 123;
    static std::vector<int> pow;
    static constexpr int base = 233;
    std::vector<int> pre;
    PolyHash(const std::string &s) : pre(s.size() + 1) {
        assert(base < mod);
        int n = s.size();
        while (pow.size() <= n) {
            pow.push_back(1LL * pow.back() * base % mod);
        }
        for (int i = 0; i < n; i++) {
            pre[i + 1] = (1LL * pre[i] * base + s[i]) % mod;
        }
    }
    int get_hash() {
        return pre.back();
    }
    int substr(int pos, int len) {
        return (pre[pos + len] - 1LL * pre[pos] * pow[len] % mod + mod) % mod;
    }
};
std::vector<int> PolyHash::pow{1};
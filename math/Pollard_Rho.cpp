#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct Pollard_Rho {
private:
    uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod) {
        assert(a < mod && b < mod);
        if (mod <= 1LLU << 32) return a * b % mod;
        if (mod <= 1LLU << 63) {
            uint64_t q = uint64_t((long double) a * b / mod);
            uint64_t result = a * b - q * mod;
            if (result > 1LLU << 63) {
                result += mod;
            } else if (result >= mod) {
                result -= mod;
            }
            return result;
        }
    #ifdef __SIZEOF_INT128__
        return uint64_t(__uint128_t(a) * b % mod);
    #endif
        assert(false);
    }
    uint64_t mod_pow64(uint64_t a, uint64_t b, uint64_t mod) {
        uint64_t result = 1;
        for (; b; b >>= 1, a = mod_mul64(a, a, mod)) {
            if (b & 1) result = mod_mul64(result, a, mod);
        }
        return result;
    }
    bool miller_rabin(uint64_t n) {
        if (n < 2) return false;
        // Check small primes.
        for (uint64_t p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29}) {
            if (n % p == 0) return n == p;
        }
        // https://miller-rabin.appspot.com/
        auto get_miller_rabin_bases = [&]() -> vector<uint64_t> {
            if (n < 341531) return {9345883071009581737LLU};
            if (n < 1050535501) return {336781006125, 9639812373923155};
            if (n < 350269456337) return {4230279247111683200, 14694767155120705706LLU, 16641139526367750375LLU};
            if (n < 55245642489451) return {2, 141889084524735, 1199124725622454117, 11096072698276303650LLU};
            if (n < 7999252175582851) return {2, 4130806001517, 149795463772692060, 186635894390467037, 3967304179347715805};
            if (n < 585226005592931977) return {2, 123635709730000, 9233062284813009, 43835965440333360, 761179012939631437, 1263739024124850375};
            return {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
        };
        int r = __builtin_ctzll(n - 1);
        uint64_t d = (n - 1) >> r;
        for (uint64_t a : get_miller_rabin_bases()) {
            if (a % n == 0) continue;
            uint64_t x = mod_pow64(a % n, d, n);
            if (x == 1 || x == n - 1) continue;
            for (int i = 0; i < r - 1 && x != n - 1; i++) {
                x = mod_mul64(x, x, n);
            }
            if (x != n - 1) return false;
        }
        return true;
    }
    int64_t solve(int64_t x) {
        int64_t s = 0, t = 0;
        int64_t c = (int64_t)rand() % (x - 1) + 1;
        int step = 0, goal = 1;
        int64_t val = 1;
        for (goal = 1;; goal *= 2, s = t, val = 1) {  // 倍增优化
            for (step = 1; step <= goal; ++step) {
                t = ((__int128)t * t + c) % x;
                val = (__int128)val * abs(t - s) % x;
                if ((step % 127) == 0) {
                    int64_t d = gcd(val, x);
                    if (d > 1) return d;
                }
            }
            int64_t d = gcd(val, x);
            if (d > 1) return d;
        }
    }
    void fac(int64_t x, vector<int64_t> &ans) {
        if (x == 1) return;
        if (miller_rabin(x)) { // 如果 x 为质数
            ans.push_back(x);
            return;
        }
        int64_t p = x;
        while (p >= x) p = solve(x);  // 使用该算法
        while ((x % p) == 0) x /= p;
        fac(x, ans), fac(p, ans);  // 继续向下分解 x 和 p
    }
    
public:
    Pollard_Rho() { srand((unsigned)time(NULL)); }
    vector<int64_t> pollard_Rho(int64_t x) {
        vector<int64_t> ans;
        fac(x, ans);
        return ans;
    }
    bool isPrime(int64_t x) {
        return miller_rabin(x);
    }
};

void solve() {
    ll n;
    cin >> n;
    Pollard_Rho poll;
    auto ans = poll.pollard_Rho(n);
    ll maxx = *max_element(ans.begin(), ans.end());

    if (maxx == n) {
        cout << "Prime\n";
    } else {
        cout << maxx << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
// test problem: https://www.luogu.com.cn/problem/P4718
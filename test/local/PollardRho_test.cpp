#include <bits/stdc++.h>
#ifdef LOCAL
#include "pprint.hpp"
#else
#define debug(...)
#endif

using ll = long long;

#include "src/Math/NumberTheory/PollardRho.h"

int main() {
    uint64_t A = 2ull * 3 * 3 * 11;
    for (auto [prime, count] : OY::PollardRho::decomposite<true>(A)) {
        std::cout << prime << "^" << count << "\n";
    }
    std::cout << "\n";

    // 枚举所有的质因子，重复的质因子会被枚举多次
    OY::PollardRho::enumerate_prime_factors(A, [](uint64_t factor) { 
        std::cout << factor << "\n"; 
    });
    std::cout << "\n";

    for (auto factor : OY::PollardRho::get_factors<true>(A)) {
        std::cout << factor << "\n";
    }
    std::cout << "\n";

    OY::PollardRho::enumerate_factors(A, [](uint64_t factor) { 
        std::cout << factor << "\n"; 
    });
    std::cout << "\n";

    return 0;
}
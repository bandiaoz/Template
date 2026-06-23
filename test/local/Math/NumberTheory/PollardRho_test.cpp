#include "src/Math/NumberTheory/PollardRho.h"
#include <gtest/gtest.h>

TEST(PollardRho, EnumeratePrimeFactors) {
    std::vector<uint64_t> res;
    OY::PollardRho::enumerate_prime_factors(60ULL, [&](uint64_t p) { res.push_back(p); });
    std::sort(res.begin(), res.end());
    EXPECT_EQ(res, (std::vector<uint64_t>{2, 2, 3, 5}));
}

TEST(PollardRho, DecompositeSorted) {
    // 360 = 2^3 * 3^2 * 5
    auto pairs = OY::PollardRho::decomposite<true>(360ULL);
    EXPECT_TRUE(pairs.size() == 3);
    EXPECT_TRUE(pairs[0].m_prime == 2);
    EXPECT_TRUE(pairs[0].m_count == 3);
    EXPECT_TRUE(pairs[1].m_prime == 3);
    EXPECT_TRUE(pairs[1].m_count == 2);
    EXPECT_TRUE(pairs[2].m_prime == 5);
    EXPECT_TRUE(pairs[2].m_count == 1);
}

TEST(PollardRho, GetFactorsSorted) {
    auto factors = OY::PollardRho::get_factors<true>(360ULL);
    std::vector<uint64_t> expected;
    for (int i = 1; i <= 360; i++) {
        if (360 % i == 0) {
            expected.push_back(i);
        }
    }
    EXPECT_TRUE(factors == expected);
}

TEST(PollardRho, EnumerateFactorsViaCallback) {
    std::vector<uint64_t> factors;
    OY::PollardRho::enumerate_factors(360ULL, [&](uint64_t f) { factors.push_back(f); });
    std::sort(factors.begin(), factors.end());
    std::vector<uint64_t> expected;
    for (int i = 1; i <= 360; i++) {
        if (360 % i == 0) {
            expected.push_back(i);
        }
    }
    EXPECT_TRUE(factors == expected);
}

TEST(PollardRho, GetEulerPhi) {
    EXPECT_TRUE(OY::PollardRho::get_Euler_Phi(1ULL) == 1ULL);
    EXPECT_TRUE(OY::PollardRho::get_Euler_Phi(10ULL) == 4ULL);
    EXPECT_TRUE(OY::PollardRho::get_Euler_Phi(11ULL) == 10ULL);
    EXPECT_TRUE(OY::PollardRho::get_Euler_Phi(36ULL) == 12ULL);
}

TEST(PollardRho, PickNonTrivialFactor) {
    uint64_t n = 9ULL;  // 9 = 3*3
    uint64_t f = OY::PollardRho::pick(n);
    EXPECT_TRUE(f > 1);
    EXPECT_TRUE(f < n);
    EXPECT_TRUE(n % f == 0);
}

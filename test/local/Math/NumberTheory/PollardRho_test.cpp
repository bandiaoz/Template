#include "src/Math/NumberTheory/PollardRho.h"
#include "third_party/catch/catch_amalgamated.hpp"

TEST_CASE("enumerate_prime_factors", "[PollardRho]") {
    std::vector<uint64_t> res;
    OY::PollardRho::enumerate_prime_factors(60ULL, [&](uint64_t p) { res.push_back(p); });
    std::sort(res.begin(), res.end());
    REQUIRE(res == std::vector<uint64_t>{2, 2, 3, 5});
}

TEST_CASE("decomposite sorted", "[PollardRho]") {
    // 360 = 2^3 * 3^2 * 5
    auto pairs = OY::PollardRho::decomposite<true>(360ULL);
    REQUIRE(pairs.size() == 3);
    REQUIRE(pairs[0].m_prime == 2);
    REQUIRE(pairs[0].m_count == 3);
    REQUIRE(pairs[1].m_prime == 3);
    REQUIRE(pairs[1].m_count == 2);
    REQUIRE(pairs[2].m_prime == 5);
    REQUIRE(pairs[2].m_count == 1);
}

TEST_CASE("get_factors sorted", "[PollardRho]") {
    auto factors = OY::PollardRho::get_factors<true>(360ULL);
    std::vector<uint64_t> expected;
    for (int i = 1; i <= 360; i++) {
        if (360 % i == 0) {
            expected.push_back(i);
        }
    }
    REQUIRE(factors == expected);
}

TEST_CASE("enumerate_factors via callback", "[PollardRho]") {
    std::vector<uint64_t> factors;
    OY::PollardRho::enumerate_factors(360ULL, [&](uint64_t f) { factors.push_back(f); });
    std::sort(factors.begin(), factors.end());
    std::vector<uint64_t> expected;
    for (int i = 1; i <= 360; i++) {
        if (360 % i == 0) {
            expected.push_back(i);
        }
    }
    REQUIRE(factors == expected);
}

TEST_CASE("get_Euler_Phi", "[PollardRho]") {
    REQUIRE(OY::PollardRho::get_Euler_Phi(1ULL) == 1ULL);
    REQUIRE(OY::PollardRho::get_Euler_Phi(10ULL) == 4ULL);
    REQUIRE(OY::PollardRho::get_Euler_Phi(11ULL) == 10ULL);
    REQUIRE(OY::PollardRho::get_Euler_Phi(36ULL) == 12ULL);
}

TEST_CASE("pick non-trivial factor", "[PollardRho]") {
    uint64_t n = 9ULL;  // 9 = 3*3
    uint64_t f = OY::PollardRho::pick(n);
    REQUIRE(f > 1);
    REQUIRE(f < n);
    REQUIRE(n % f == 0);
}
#include "src/Math/Modular/StaticModInt32.h"
#include "third_party/catch/catch_amalgamated.hpp"

using Z = OY::mint1000000007;

TEST_CASE("Default construction and value()", "[StaticModInt32]") {
    Z a = 0;
    REQUIRE(a.val() == 0);
    Z b(5);
    REQUIRE(b.val() == 5);
    Z c(-1);
    REQUIRE(c.val() == Z::mod() - 1);
    REQUIRE(Z::identity().val() == 1);
}

TEST_CASE("Arithmetic operations", "[StaticModInt32]") {
    Z a(3), b(4);
    REQUIRE((a + b).val() == 7);
    REQUIRE((b - a).val() == 1);
    REQUIRE((a * b).val() == 12);
    // division: (b / a) * a == b
    REQUIRE((b / a * a).val() == b.val());
}

TEST_CASE("Power and inverse (Fermat)", "[StaticModInt32]") {
    REQUIRE(Z(2).pow(10).val() == static_cast<uint32_t>(1024 % Z::mod()));
    auto inv2 = Z(2).inv();
    REQUIRE((inv2 * Z(2)).val() == 1);
}

TEST_CASE("Composite modulus inverse (exgcd)", "[StaticModInt32]") {
    using C = OY::StaticModInt32<10, false>;
    C x(3);
    auto inv = x.inv();
    REQUIRE((inv * x).val() == 1);
    REQUIRE(inv.val() == 7);
}
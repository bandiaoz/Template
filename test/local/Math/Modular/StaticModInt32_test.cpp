#include "src/Math/Modular/StaticModInt32.h"
#include <gtest/gtest.h>

using Z = OY::mint1000000007;

TEST(StaticModInt32, DefaultConstructionAndValue) {
    Z a = 0;
    EXPECT_TRUE(a.val() == 0);
    Z b(5);
    EXPECT_TRUE(b.val() == 5);
    Z c(-1);
    EXPECT_TRUE(c.val() == Z::mod() - 1);
    EXPECT_TRUE(Z::identity().val() == 1);
}

TEST(StaticModInt32, ArithmeticOperations) {
    Z a(3), b(4);
    EXPECT_TRUE((a + b).val() == 7);
    EXPECT_TRUE((b - a).val() == 1);
    EXPECT_TRUE((a * b).val() == 12);
    // division: (b / a) * a == b
    EXPECT_TRUE((b / a * a).val() == b.val());
}

TEST(StaticModInt32, PowerAndInverseFermat) {
    EXPECT_TRUE(Z(2).pow(10).val() == static_cast<uint32_t>(1024 % Z::mod()));
    auto inv2 = Z(2).inv();
    EXPECT_TRUE((inv2 * Z(2)).val() == 1);
}

TEST(StaticModInt32, CompositeModulusInverseExgcd) {
    using C = OY::StaticModInt32<10, false>;
    C x(3);
    auto inv = x.inv();
    EXPECT_TRUE((inv * x).val() == 1);
    EXPECT_TRUE(inv.val() == 7);
}
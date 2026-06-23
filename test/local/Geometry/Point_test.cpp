#include "src/Geometry/Point.h"
#include <gtest/gtest.h>

using IP = OY::GEO::Point<int>;
using FP = OY::GEO::Point<OY::GEO::real_type>;

TEST(GeometryPoint, PointArithmeticWorksForIntegerCoordinates) {
    IP a(1, 2), b(3, -4);
    EXPECT_TRUE(a + b == IP(4, -2));
    EXPECT_TRUE(b - a == IP(2, -6));
    EXPECT_TRUE(2 * a == IP(2, 4));
    EXPECT_TRUE(a * 3 == IP(3, 6));
    EXPECT_TRUE(-a == IP(-1, -2));
    EXPECT_TRUE(OY::GEO::dot(a, b) == -5);
    EXPECT_TRUE(OY::GEO::cross(a, b) == -10);

    IP u(1000000000, 0), v(0, 1000000000);
    EXPECT_TRUE(OY::GEO::cross(u, v) == 1000000000000000000LL);
    EXPECT_TRUE((u + v).norm2() == 2000000000000000000LL);
}

TEST(GeometryPoint, PointSupportsFloatingGeometryHelpers) {
    FP p(3, 4);
    EXPECT_TRUE(p.norm2() == 25);
    EXPECT_TRUE(std::abs((double)p.norm() - 5.0) < 1e-12);

    auto unit = OY::GEO::normalize(p);
    EXPECT_TRUE(std::abs((double)unit.x - 0.6) < 1e-12);
    EXPECT_TRUE(std::abs((double)unit.y - 0.8) < 1e-12);

    auto q = OY::GEO::rotate(FP(1, 0), std::acos(-1.0L) / 2);
    EXPECT_TRUE(std::abs((double)q.x) < 1e-12);
    EXPECT_TRUE(std::abs((double)q.y - 1.0) < 1e-12);

    auto r = OY::GEO::rotate_around(FP(2, 1), FP(1, 1), std::acos(-1.0L) / 2);
    EXPECT_TRUE(std::abs((double)r.x - 1.0) < 1e-12);
    EXPECT_TRUE(std::abs((double)r.y - 2.0) < 1e-12);
}

TEST(GeometryPoint, PointSupportsAngleAndDistanceHelpers) {
    EXPECT_TRUE(std::abs((double)OY::GEO::angle(IP(1, 0), IP(0, 1)) -
                     std::acos(-1.0) / 2) < 1e-12);
    EXPECT_TRUE(std::abs((double)OY::GEO::angle(IP(1, 0), IP(0, 0), IP(0, 1)) -
                     std::acos(-1.0) / 2) < 1e-12);
    EXPECT_TRUE(std::abs((double)OY::GEO::distance(IP(0, 0), IP(3, 4)) - 5.0) < 1e-12);
}

#include "src/Geometry/Point.h"
#include "third_party/catch/catch_amalgamated.hpp"

using IP = OY::GEO::Point<int>;
using FP = OY::GEO::Point<OY::GEO::real_type>;

TEST_CASE("Point arithmetic works for integer coordinates", "[Geometry][Point]") {
    IP a(1, 2), b(3, -4);
    REQUIRE(a + b == IP(4, -2));
    REQUIRE(b - a == IP(2, -6));
    REQUIRE(2 * a == IP(2, 4));
    REQUIRE(a * 3 == IP(3, 6));
    REQUIRE(-a == IP(-1, -2));
    REQUIRE(OY::GEO::dot(a, b) == -5);
    REQUIRE(OY::GEO::cross(a, b) == -10);

    IP u(1000000000, 0), v(0, 1000000000);
    REQUIRE(OY::GEO::cross(u, v) == 1000000000000000000LL);
    REQUIRE((u + v).norm2() == 2000000000000000000LL);
}

TEST_CASE("Point supports floating geometry helpers", "[Geometry][Point]") {
    FP p(3, 4);
    REQUIRE(p.norm2() == 25);
    REQUIRE(std::abs((double)p.norm() - 5.0) < 1e-12);

    auto unit = OY::GEO::normalize(p);
    REQUIRE(std::abs((double)unit.x - 0.6) < 1e-12);
    REQUIRE(std::abs((double)unit.y - 0.8) < 1e-12);

    auto q = OY::GEO::rotate(FP(1, 0), std::acos(-1.0L) / 2);
    REQUIRE(std::abs((double)q.x) < 1e-12);
    REQUIRE(std::abs((double)q.y - 1.0) < 1e-12);

    auto r = OY::GEO::rotate_around(FP(2, 1), FP(1, 1), std::acos(-1.0L) / 2);
    REQUIRE(std::abs((double)r.x - 1.0) < 1e-12);
    REQUIRE(std::abs((double)r.y - 2.0) < 1e-12);
}

TEST_CASE("Point line intersection returns real coordinates", "[Geometry][Point]") {
    auto p = OY::GEO::line_intersection(IP(0, 0), IP(2, 2), IP(0, 2), IP(2, 0));
    REQUIRE(std::abs((double)p.x - 1.0) < 1e-12);
    REQUIRE(std::abs((double)p.y - 1.0) < 1e-12);

    REQUIRE(std::abs((double)OY::GEO::angle(IP(1, 0), IP(0, 1)) -
                     std::acos(-1.0) / 2) < 1e-12);
    REQUIRE(std::abs((double)OY::GEO::angle(IP(1, 0), IP(0, 0), IP(0, 1)) -
                     std::acos(-1.0) / 2) < 1e-12);
    REQUIRE(std::abs((double)OY::GEO::distance(IP(0, 0), IP(3, 4)) - 5.0) < 1e-12);
}

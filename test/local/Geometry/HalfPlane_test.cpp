#include "src/Geometry/HalfPlane.h"
#include "third_party/catch/catch_amalgamated.hpp"

using IP = OY::GEO::Point<int>;
using Poly = OY::GEO::Polygon<int>;
using HP = OY::GEO::HalfPlane<int>;

static bool close_to(OY::GEO::real_type a, OY::GEO::real_type b) {
    return std::abs((double)(a - b)) < 1e-10;
}

TEST_CASE("HalfPlane keeps the left side of directed line", "[Geometry][HalfPlane]") {
    HP h(IP(0, 0), IP(4, 0));
    REQUIRE(h.relation(IP(1, 2)) == 1);
    REQUIRE(h.relation(IP(1, -2)) == -1);
    REQUIRE(h.contains(IP(2, 0)));
    REQUIRE(!h.contains(IP(2, 0), false));
    REQUIRE(h.contains(IP(1, 2)));
    REQUIRE(!h.contains(IP(1, -2)));
}

TEST_CASE("cut_polygon clips by one half plane", "[Geometry][HalfPlane]") {
    Poly square{IP(-1, -1), IP(1, -1), IP(1, 1), IP(-1, 1)};
    HP keep_right(IP(0, 1), IP(0, -1));

    auto res = OY::GEO::cut_polygon(square, keep_right);
    REQUIRE(res.size() == 4);
    REQUIRE(close_to(res.area(), 2));
    for (auto &p : res.p) REQUIRE(p.x >= -OY::GEO::EPS);
}

TEST_CASE("half plane intersection handles bounded and unbounded cases", "[Geometry][HalfPlane]") {
    std::vector<HP> square{
        HP(IP(-1, -1), IP(1, -1)),
        HP(IP(1, -1), IP(1, 1)),
        HP(IP(1, 1), IP(-1, 1)),
        HP(IP(-1, 1), IP(-1, -1))
    };
    auto res = OY::GEO::half_plane_intersection(square);
    REQUIRE(res.size() == 4);
    REQUIRE(close_to(res.area(), 4));

    square.push_back(HP(IP(0, -1), IP(0, 1)));
    res = OY::GEO::half_plane_intersection(square);
    REQUIRE(res.size() == 4);
    REQUIRE(close_to(res.area(), 2));
    for (auto &p : res.p) REQUIRE(p.x <= OY::GEO::EPS);

    std::vector<HP> empty{
        HP(IP(1, 1), IP(1, -1)),
        HP(IP(0, -1), IP(0, 1)),
        HP(IP(-1, -1), IP(2, -1)),
        HP(IP(2, 1), IP(-1, 1))
    };
    REQUIRE(OY::GEO::half_plane_intersection(empty).empty());

    std::vector<HP> unbounded{
        HP(IP(0, 1), IP(0, 0)),
        HP(IP(0, 0), IP(1, 0)),
        HP(IP(0, 1), IP(1, 0))
    };
    REQUIRE(OY::GEO::half_plane_intersection(unbounded).empty());

    auto clipped = OY::GEO::half_plane_intersection_with_box(unbounded, 10);
    REQUIRE(!clipped.empty());
    for (auto &p : clipped.p) {
        REQUIRE(p.x >= -OY::GEO::EPS);
        REQUIRE(p.y >= -OY::GEO::EPS);
        REQUIRE(p.x + p.y >= 1 - OY::GEO::EPS);
    }
}

TEST_CASE("polygon kernel accepts both orientations", "[Geometry][HalfPlane]") {
    Poly ccw{IP(0, 0), IP(4, 0), IP(4, 3), IP(0, 3)};
    Poly cw{IP(0, 0), IP(0, 3), IP(4, 3), IP(4, 0)};

    auto a = OY::GEO::polygon_kernel(ccw);
    auto b = OY::GEO::polygon_kernel(cw);
    REQUIRE(a.size() == 4);
    REQUIRE(b.size() == 4);
    REQUIRE(close_to(a.area(), 12));
    REQUIRE(close_to(b.area(), 12));
}

TEST_CASE("convex_intersection returns the overlap polygon", "[Geometry][HalfPlane]") {
    Poly a{IP(0, 0), IP(2, 0), IP(2, 2), IP(0, 2)};
    Poly b{IP(1, 1), IP(3, 1), IP(3, 3), IP(1, 3)};

    auto res = OY::GEO::convex_intersection(a, b);
    REQUIRE(res.size() == 4);
    REQUIRE(close_to(res.area(), 1));
    for (auto &p : res.p) {
        REQUIRE(p.x >= 1 - OY::GEO::EPS);
        REQUIRE(p.x <= 2 + OY::GEO::EPS);
        REQUIRE(p.y >= 1 - OY::GEO::EPS);
        REQUIRE(p.y <= 2 + OY::GEO::EPS);
    }
}

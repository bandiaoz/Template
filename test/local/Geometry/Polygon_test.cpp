#include "src/Geometry/Polygon.h"
#include "third_party/catch/catch_amalgamated.hpp"

using IP = OY::GEO::Point<int>;
using RP = OY::GEO::Point<OY::GEO::real_type>;
using RL = OY::GEO::Line<OY::GEO::real_type>;
using Poly = OY::GEO::Polygon<int>;

TEST_CASE("Polygon basic measurements and point relation", "[Geometry][Polygon]") {
    Poly rect{IP(0, 0), IP(4, 0), IP(4, 3), IP(0, 3)};
    REQUIRE(rect.size() == 4);
    REQUIRE(rect.signed_area2() == 24);
    REQUIRE(std::abs((double)rect.area() - 12.0) < 1e-12);
    REQUIRE(std::abs((double)rect.perimeter() - 14.0) < 1e-12);

    auto c = rect.centroid();
    REQUIRE(std::abs((double)c.x - 2.0) < 1e-12);
    REQUIRE(std::abs((double)c.y - 1.5) < 1e-12);

    REQUIRE(rect.relation(IP(2, 1)) == 1);
    REQUIRE(rect.relation(IP(4, 2)) == 2);
    REQUIRE(rect.relation(IP(5, 2)) == 0);
    REQUIRE(rect.contains(IP(4, 2)));
    REQUIRE(!rect.contains(IP(4, 2), false));
    REQUIRE(rect.contains_segment(OY::GEO::Line<int>(IP(1, 1), IP(3, 2))));
    REQUIRE(!rect.contains_segment(OY::GEO::Line<int>(IP(1, 1), IP(5, 2))));
    REQUIRE(rect.contains_segment(OY::GEO::Line<int>(IP(0, 0), IP(4, 0))));
    REQUIRE(!rect.contains_segment(OY::GEO::Line<int>(IP(0, 0), IP(4, 0)), false));

    REQUIRE(rect.boundary_lattice_points() == 14);
    REQUIRE(rect.interior_lattice_points() == 6);
}

TEST_CASE("Polygon convexity handles collinear points", "[Geometry][Polygon]") {
    Poly with_collinear{IP(0, 0), IP(2, 0), IP(4, 0), IP(4, 3), IP(0, 3)};
    REQUIRE(with_collinear.is_convex());
    REQUIRE(!with_collinear.is_convex(false));

    Poly concave{IP(0, 0), IP(4, 0), IP(2, 1), IP(4, 4), IP(0, 4)};
    REQUIRE(!concave.is_convex());
    REQUIRE(concave.relation(IP(1, 1)) == 1);
    REQUIRE(concave.relation(IP(3, 2)) == 0);

    Poly u_shape{
        IP(0, 0), IP(4, 0), IP(4, 4), IP(3, 4),
        IP(3, 1), IP(1, 1), IP(1, 4), IP(0, 4)
    };
    REQUIRE(u_shape.contains(RP(0.5L, 2)));
    REQUIRE(u_shape.contains(RP(3.5L, 2)));
    REQUIRE(!u_shape.contains(RP(2, 2)));
    REQUIRE(!u_shape.contains_segment(RL(RP(0.5L, 2), RP(3.5L, 2))));
    REQUIRE(u_shape.contains_segment(OY::GEO::Line<int>(IP(1, 1), IP(3, 1))));
}

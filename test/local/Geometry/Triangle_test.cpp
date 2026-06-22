#include "src/Geometry/Triangle.h"
#include "third_party/catch/catch_amalgamated.hpp"

using IP = OY::GEO::Point<int>;
using FP = OY::GEO::Point<OY::GEO::real_type>;
using IT = OY::GEO::Triangle<int>;
using RT = OY::GEO::Triangle<OY::GEO::real_type>;

static bool close(OY::GEO::real_type a, OY::GEO::real_type b, OY::GEO::real_type eps = 1e-10L) {
    return std::abs((double)(a - b)) <= (double)eps;
}

static bool close_point(const FP &a, const FP &b, OY::GEO::real_type eps = 1e-10L) {
    return close(a.x, b.x, eps) && close(a.y, b.y, eps);
}

TEST_CASE("Triangle measurements and point relation", "[Geometry][Triangle]") {
    IT tri(IP(0, 0), IP(4, 0), IP(0, 3));
    REQUIRE(tri.signed_area2() == 12);
    REQUIRE(!tri.is_degenerate());
    REQUIRE(close(tri.area(), 6));
    REQUIRE(close(tri.perimeter(), 12));
    REQUIRE(close(tri.side_a(), 5));
    REQUIRE(close(tri.side_b(), 3));
    REQUIRE(close(tri.side_c(), 4));

    REQUIRE(tri.relation(IP(1, 1)) == 1);
    REQUIRE(tri.relation(IP(2, 0)) == 2);
    REQUIRE(tri.relation(IP(4, 3)) == 0);
    REQUIRE(tri.contains(IP(2, 0)));
    REQUIRE(!tri.contains(IP(2, 0), false));

    IT rev(IP(0, 0), IP(0, 3), IP(4, 0));
    REQUIRE(rev.signed_area2() == -12);
    REQUIRE(rev.relation(IP(1, 1)) == 1);
    REQUIRE(close(rev.to_polygon().area(), 6));

    IT line(IP(0, 0), IP(2, 0), IP(5, 0));
    REQUIRE(line.is_degenerate());
    REQUIRE(line.relation(IP(3, 0)) == 2);
    REQUIRE(line.relation(IP(6, 0)) == 0);
}

TEST_CASE("Triangle classical centers", "[Geometry][Triangle]") {
    IT tri(IP(0, 0), IP(4, 0), IP(0, 3));

    REQUIRE(close_point(tri.centroid(), FP(4.0L / 3, 1)));
    REQUIRE(close_point(tri.circumcenter(), FP(2, 1.5L)));
    REQUIRE(close_point(tri.orthocenter(), FP(0, 0)));
    REQUIRE(close_point(tri.incenter(), FP(1, 1)));

    auto out = tri.circumcircle();
    REQUIRE(close_point(out.o, FP(2, 1.5L)));
    REQUIRE(close(out.r, 2.5L));

    auto in = tri.incircle();
    REQUIRE(close_point(in.o, FP(1, 1)));
    REQUIRE(close(in.r, 1));
}

TEST_CASE("Triangle Fermat point", "[Geometry][Triangle]") {
    OY::GEO::real_type root3 = std::sqrt((OY::GEO::real_type)3);
    RT equilateral(FP(0, 0), FP(2, 0), FP(1, root3));
    FP center(1, root3 / 3);

    REQUIRE(close_point(equilateral.centroid(), center));
    REQUIRE(close_point(equilateral.incenter(), center));
    REQUIRE(close_point(equilateral.circumcenter(), center));
    REQUIRE(close_point(equilateral.fermat_point(), center));

    IT obtuse(IP(0, 0), IP(1, 0), IP(-1, 1));
    REQUIRE(close_point(obtuse.fermat_point(), FP(0, 0)));

    IT line(IP(0, 0), IP(5, 0), IP(2, 0));
    REQUIRE(close_point(line.fermat_point(), FP(2, 0)));
}

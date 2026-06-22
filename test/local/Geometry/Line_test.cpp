#include "src/Geometry/Line.h"
#include "third_party/catch/catch_amalgamated.hpp"

using IP = OY::GEO::Point<int>;
using IL = OY::GEO::Line<int>;
using FL = OY::GEO::Line<OY::GEO::real_type>;

TEST_CASE("Line basic relation and point containment", "[Geometry][Line]") {
    IL l(IP(0, 0), IP(4, 0));
    REQUIRE(l.vec() == IP(4, 0));
    REQUIRE(l.norm2() == 16);
    REQUIRE(std::abs((double)l.norm() - 4.0) < 1e-12);
    REQUIRE(l.relation(IP(2, 1)) == 1);
    REQUIRE(l.relation(IP(2, -1)) == -1);
    REQUIRE(l.point_on_line(IP(7, 0)));
    REQUIRE(l.point_on_segment(IP(4, 0)));
    REQUIRE(l.point_on_segment_strict(IP(2, 0)));
    REQUIRE(!l.point_on_segment_strict(IP(4, 0)));
    REQUIRE(!l.point_on_segment(IP(5, 0)));

    IL big(IP(0, 0), IP(1000000000, 0));
    REQUIRE(big.relation(IP(1000000000, 1000000000)) == 1);

    FL almost(IP(0, 0), IP(1, 0));
    REQUIRE(almost.point_on_line({0.5L, 1e-13L}));
}

TEST_CASE("Line projection reflection and distances", "[Geometry][Line]") {
    IL l(IP(0, 0), IP(4, 0));
    auto proj = l.projection(IP(2, 3));
    REQUIRE(std::abs((double)proj.x - 2.0) < 1e-12);
    REQUIRE(std::abs((double)proj.y) < 1e-12);

    auto refl = l.reflection(IP(2, 3));
    REQUIRE(std::abs((double)refl.x - 2.0) < 1e-12);
    REQUIRE(std::abs((double)refl.y + 3.0) < 1e-12);

    REQUIRE(std::abs((double)l.distance_to_line(IP(2, 3)) - 3.0) < 1e-12);
    REQUIRE(std::abs((double)l.distance_to_segment(IP(6, 3)) -
                     std::sqrt(13.0)) < 1e-12);

    IL point_segment(IP(1, 1), IP(1, 1));
    REQUIRE(std::abs((double)point_segment.distance_to_segment(IP(4, 5)) - 5.0) < 1e-12);
}

TEST_CASE("Line intersections and segment classification", "[Geometry][Line]") {
    IL x(IP(0, 0), IP(4, 0));
    IL y(IP(2, -2), IP(2, 2));
    REQUIRE(OY::GEO::perpendicular(x, y));
    REQUIRE(!OY::GEO::parallel(x, y));

    auto p = OY::GEO::line_intersection(x, y);
    REQUIRE(std::abs((double)p.x - 2.0) < 1e-12);
    REQUIRE(std::abs((double)p.y) < 1e-12);

    auto proper = OY::GEO::segment_intersection(
        IL(IP(0, 0), IP(4, 4)), IL(IP(0, 4), IP(4, 0)));
    REQUIRE(proper.type == 1);
    REQUIRE(std::abs((double)proper.p1.x - 2.0) < 1e-12);
    REQUIRE(std::abs((double)proper.p1.y - 2.0) < 1e-12);

    auto touch = OY::GEO::segment_intersection(
        IL(IP(0, 0), IP(2, 0)), IL(IP(2, 0), IP(2, 2)));
    REQUIRE(touch.type == 3);
    REQUIRE(std::abs((double)touch.p1.x - 2.0) < 1e-12);
    REQUIRE(std::abs((double)touch.p1.y) < 1e-12);

    auto overlap = OY::GEO::segment_intersection(
        IL(IP(0, 0), IP(4, 0)), IL(IP(2, 0), IP(6, 0)));
    REQUIRE(overlap.type == 2);
    REQUIRE(std::abs((double)overlap.p1.x - 2.0) < 1e-12);
    REQUIRE(std::abs((double)overlap.p2.x - 4.0) < 1e-12);

    auto none = OY::GEO::segment_intersection(
        IL(IP(0, 0), IP(1, 0)), IL(IP(2, 0), IP(3, 0)));
    REQUIRE(none.type == 0);

    auto point_touch = OY::GEO::segment_intersection(
        IL(IP(1, 1), IP(1, 1)), IL(IP(0, 0), IP(2, 2)));
    REQUIRE(point_touch.type == 3);
    REQUIRE(std::abs((double)point_touch.p1.x - 1.0) < 1e-12);
    REQUIRE(std::abs((double)point_touch.p1.y - 1.0) < 1e-12);

    auto point_miss = OY::GEO::segment_intersection(
        IL(IP(1, 1), IP(1, 1)), IL(IP(0, 0), IP(3, 2)));
    REQUIRE(point_miss.type == 0);

    auto diagonal_overlap = OY::GEO::segment_intersection(
        IL(IP(0, 4), IP(4, 0)), IL(IP(3, 1), IP(1, 3)));
    REQUIRE(diagonal_overlap.type == 2);
    REQUIRE(std::abs((double)diagonal_overlap.p1.x - 1.0) < 1e-12);
    REQUIRE(std::abs((double)diagonal_overlap.p1.y - 3.0) < 1e-12);
    REQUIRE(std::abs((double)diagonal_overlap.p2.x - 3.0) < 1e-12);
    REQUIRE(std::abs((double)diagonal_overlap.p2.y - 1.0) < 1e-12);
}

TEST_CASE("Line translation and segment distance", "[Geometry][Line]") {
    IL x(IP(0, 0), IP(4, 0));
    auto up = x.translated(2);
    REQUIRE(std::abs((double)up.s.x) < 1e-12);
    REQUIRE(std::abs((double)up.s.y - 2.0) < 1e-12);
    REQUIRE(std::abs((double)up.t.x - 4.0) < 1e-12);
    REQUIRE(std::abs((double)up.t.y - 2.0) < 1e-12);

    FL a({0, 0}, {4, 0});
    FL b({0, 3}, {4, 3});
    REQUIRE(std::abs((double)OY::GEO::distance(a, b) - 3.0) < 1e-12);

    IL point_segment(IP(1, 1), IP(1, 1));
    IL far_segment(IP(4, 5), IP(4, 6));
    REQUIRE(std::abs((double)OY::GEO::distance(point_segment, far_segment) - 5.0) < 1e-12);
}

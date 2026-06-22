#include "src/Geometry/Circle.h"
#include "third_party/catch/catch_amalgamated.hpp"

using IP = OY::GEO::Point<int>;
using FP = OY::GEO::Point<OY::GEO::real_type>;
using IL = OY::GEO::Line<int>;
using IC = OY::GEO::Circle<int>;
using RC = OY::GEO::Circle<OY::GEO::real_type>;
using Poly = OY::GEO::Polygon<int>;
using CirclePointRelation = OY::GEO::CirclePointRelation;
using CircleRelation = OY::GEO::CircleRelation;
using LineCircleRelation = OY::GEO::LineCircleRelation;

bool close(OY::GEO::real_type a, OY::GEO::real_type b, OY::GEO::real_type eps = 1e-10L) {
    return std::abs((double)(a - b)) <= (double)eps;
}

bool close_point(const FP &a, const FP &b, OY::GEO::real_type eps = 1e-10L) {
    return close(a.x, b.x, eps) && close(a.y, b.y, eps);
}

TEST_CASE("Circle basic measurements and relations", "[Geometry][Circle]") {
    IC c(IP(0, 0), 5);
    OY::GEO::real_type pi = std::acos((OY::GEO::real_type)-1);

    REQUIRE(close(c.area(), 25 * pi));
    REQUIRE(close(c.perimeter(), 10 * pi));
    REQUIRE(c.relation(IP(1, 1)) == CirclePointRelation::Inside);
    REQUIRE(c.relation(IP(3, 4)) == CirclePointRelation::Boundary);
    REQUIRE(c.relation(IP(6, 0)) == CirclePointRelation::Outside);
    REQUIRE(c.contains(IP(3, 4)));
    REQUIRE(!c.contains(IP(3, 4), false));

    REQUIRE(c.relation(IC(IP(10, 0), 5)) == CircleRelation::ExternallyTangent);
    REQUIRE(c.relation(IC(IP(6, 0), 5)) == CircleRelation::Intersect);
    REQUIRE(c.relation(IC(IP(3, 0), 2)) == CircleRelation::InternallyTangent);
    REQUIRE(c.relation(IC(IP(0, 0), 2)) == CircleRelation::Contain);
    REQUIRE(c.relation(IC(IP(0, 0), 5)) == CircleRelation::Same);
    REQUIRE(c.contains(IC(IP(0, 0), 2)));
    REQUIRE(!c.contains(IC(IP(3, 0), 2), false));

    REQUIRE(close_point(c.nearest_point(IP(10, 0)), FP(5, 0)));
    REQUIRE(close_point(c.nearest_point(IP(0, 0)), FP(5, 0)));
}

TEST_CASE("Circle line and segment intersections", "[Geometry][Circle]") {
    IC c(IP(0, 0), 5);

    auto ps = c.line_intersections(IL(IP(-10, 3), IP(10, 3)));
    REQUIRE(ps.size() == 2);
    REQUIRE(close_point(ps[0], FP(-4, 3)));
    REQUIRE(close_point(ps[1], FP(4, 3)));

    auto tangent = c.line_intersections(IL(IP(-10, 5), IP(10, 5)));
    REQUIRE(tangent.size() == 1);
    REQUIRE(close_point(tangent[0], FP(0, 5)));
    REQUIRE(c.relation_to_line(IL(IP(-10, 6), IP(10, 6))) == LineCircleRelation::Separate);

    auto chord = c.segment_intersections(IL(IP(-10, 0), IP(10, 0)));
    REQUIRE(chord.size() == 2);
    REQUIRE(close_point(chord[0], FP(-5, 0)));
    REQUIRE(close_point(chord[1], FP(5, 0)));

    auto half = c.segment_intersections(IL(IP(-10, 0), IP(0, 0)));
    REQUIRE(half.size() == 1);
    REQUIRE(close_point(half[0], FP(-5, 0)));

    auto miss = c.segment_intersections(IL(IP(-10, 0), IP(-6, 0)));
    REQUIRE(miss.empty());
    REQUIRE(close(c.length_inside_segment(IL(IP(-10, 0), IP(10, 0))), 10));
    REQUIRE(close(c.length_inside_segment(IL(IP(-10, 0), IP(0, 0))), 5));
}

TEST_CASE("Circle-circle intersections, tangents and areas", "[Geometry][Circle]") {
    IC a(IP(0, 0), 5), b(IP(6, 0), 5);
    auto ps = a.circle_intersections(b);
    REQUIRE(ps.size() == 2);
    REQUIRE(close(ps[0].x, 3));
    REQUIRE(close(std::abs((double)ps[0].y), 4));
    REQUIRE(close(ps[1].x, 3));
    REQUIRE(close(std::abs((double)ps[1].y), 4));

    auto tangent = a.circle_intersections(IC(IP(10, 0), 5));
    REQUIRE(tangent.size() == 1);
    REQUIRE(close_point(tangent[0], FP(5, 0)));

    auto tp = IC(IP(0, 0), 3).tangent_points(IP(5, 0));
    REQUIRE(tp.size() == 2);
    REQUIRE(close(tp[0].x, 1.8L));
    REQUIRE(close(std::abs((double)tp[0].y), 2.4L));
    REQUIRE(close(tp[1].x, 1.8L));
    REQUIRE(close(std::abs((double)tp[1].y), 2.4L));

    OY::GEO::real_type pi = std::acos((OY::GEO::real_type)-1);
    RC u(FP(0, 0), 1), v(FP(1, 0), 1);
    REQUIRE(close(u.intersection_area(v), 2 * pi / 3 - std::sqrt((OY::GEO::real_type)3) / 2));
    REQUIRE(close(u.intersection_area(RC(FP(0, 0), 1)), pi));
    REQUIRE(close(IC(IP(0, 0), 5).intersection_area(IC(IP(0, 0), 2)), 4 * pi));
}

TEST_CASE("Circle construction and minimum enclosing circle", "[Geometry][Circle]") {
    auto d = OY::GEO::circle_from_diameter(IP(0, 0), IP(2, 0));
    REQUIRE(close_point(d.o, FP(1, 0)));
    REQUIRE(close(d.r, 1));

    auto cc = OY::GEO::circumcircle(IP(0, 0), IP(0, 2), IP(2, 0));
    REQUIRE(close_point(cc.o, FP(1, 1)));
    REQUIRE(close(cc.r, std::sqrt((OY::GEO::real_type)2)));

    auto mec = OY::GEO::minimum_enclosing_circle(std::vector<IP>{
        IP(0, 0), IP(2, 0), IP(0, 2), IP(2, 2)
    });
    REQUIRE(close_point(mec.o, FP(1, 1)));
    REQUIRE(close(mec.r, std::sqrt((OY::GEO::real_type)2)));

    auto random_mec = OY::GEO::minimum_enclosing_circle(std::vector<IP>{
        IP(0, 0), IP(2, 0), IP(0, 2), IP(2, 2)
    }, true);
    REQUIRE(close_point(random_mec.o, FP(1, 1)));
    REQUIRE(close(random_mec.r, std::sqrt((OY::GEO::real_type)2)));

    auto collinear = OY::GEO::minimum_enclosing_circle(std::vector<IP>{
        IP(1, 0), IP(0, 0), IP(3, 0)
    });
    REQUIRE(close_point(collinear.o, FP(1.5L, 0)));
    REQUIRE(close(collinear.r, 1.5L));

    auto obtuse = OY::GEO::minimum_enclosing_circle(std::vector<IP>{
        IP(0, 0), IP(4, 0), IP(1, 1)
    });
    REQUIRE(close_point(obtuse.o, FP(2, 0)));
    REQUIRE(close(obtuse.r, 2));

    std::vector<IP> tri{IP(0, 0), IP(1, 1), IP(4, 0)};
    do {
        auto c = OY::GEO::minimum_enclosing_circle(tri);
        REQUIRE(close_point(c.o, FP(2, 0)));
        REQUIRE(close(c.r, 2));
    } while (std::next_permutation(tri.begin(), tri.end()));
}

TEST_CASE("Circle with polygon area and boundary length", "[Geometry][Circle]") {
    OY::GEO::real_type pi = std::acos((OY::GEO::real_type)-1);
    Poly square{IP(-2, -2), IP(2, -2), IP(2, 2), IP(-2, 2)};
    Poly square_cw{IP(-2, -2), IP(-2, 2), IP(2, 2), IP(2, -2)};
    IC unit(IP(0, 0), 1);
    IC big(IP(0, 0), 10);

    REQUIRE(close(unit.intersection_area(square), pi));
    REQUIRE(close(unit.intersection_area(square_cw), pi));
    REQUIRE(close(big.intersection_area(square), 16));
    REQUIRE(close(big.intersection_area(square_cw), 16));
    REQUIRE(close(unit.length_inside_polygon(square), 0));
    REQUIRE(close(big.length_inside_polygon(square), 16));

    Poly half{IP(0, -2), IP(2, -2), IP(2, 2), IP(0, 2)};
    REQUIRE(close(unit.intersection_area(half), pi / 2));
    REQUIRE(close(unit.length_inside_polygon(half), 2));

    Poly quadrant{IP(0, 0), IP(2, 0), IP(2, 2), IP(0, 2)};
    REQUIRE(close(unit.intersection_area(quadrant), pi / 4));
    REQUIRE(close(unit.length_inside_polygon(quadrant), 2));

    Poly tri{IP(0, 0), IP(3, 0), IP(0, 4)};
    REQUIRE(close(big.intersection_area(tri), 6));
    REQUIRE(close(unit.intersection_area(Poly{IP(2, 2), IP(3, 2), IP(2, 3)}), 0));

    IC circle(IP(0, 0), 5);
    REQUIRE(close(circle.arc_length_inside(IC(IP(0, 0), 10)), circle.perimeter()));
    REQUIRE(close(circle.arc_length_inside(IC(IP(0, 0), 3)), 0));
    REQUIRE(close(circle.arc_length_inside(IC(IP(5, 0), 5)), 10 * pi / 3));
}

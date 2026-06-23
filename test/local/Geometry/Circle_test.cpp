#include "src/Geometry/Circle.h"
#include <gtest/gtest.h>

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

TEST(GeometryCircle, CircleBasicMeasurementsAndRelations) {
    IC c(IP(0, 0), 5);
    OY::GEO::real_type pi = std::acos((OY::GEO::real_type)-1);

    EXPECT_TRUE(close(c.area(), 25 * pi));
    EXPECT_TRUE(close(c.perimeter(), 10 * pi));
    EXPECT_TRUE(c.relation(IP(1, 1)) == CirclePointRelation::Inside);
    EXPECT_TRUE(c.relation(IP(3, 4)) == CirclePointRelation::Boundary);
    EXPECT_TRUE(c.relation(IP(6, 0)) == CirclePointRelation::Outside);
    EXPECT_TRUE(c.contains(IP(3, 4)));
    EXPECT_TRUE(!c.contains(IP(3, 4), false));

    EXPECT_TRUE(c.relation(IC(IP(10, 0), 5)) == CircleRelation::ExternallyTangent);
    EXPECT_TRUE(c.relation(IC(IP(6, 0), 5)) == CircleRelation::Intersect);
    EXPECT_TRUE(c.relation(IC(IP(3, 0), 2)) == CircleRelation::InternallyTangent);
    EXPECT_TRUE(c.relation(IC(IP(0, 0), 2)) == CircleRelation::Contain);
    EXPECT_TRUE(c.relation(IC(IP(0, 0), 5)) == CircleRelation::Same);
    EXPECT_TRUE(c.contains(IC(IP(0, 0), 2)));
    EXPECT_TRUE(!c.contains(IC(IP(3, 0), 2), false));

    EXPECT_TRUE(close_point(c.nearest_point(IP(10, 0)), FP(5, 0)));
    EXPECT_TRUE(close_point(c.nearest_point(IP(0, 0)), FP(5, 0)));
}

TEST(GeometryCircle, CircleLineAndSegmentIntersections) {
    IC c(IP(0, 0), 5);

    auto ps = c.line_intersections(IL(IP(-10, 3), IP(10, 3)));
    EXPECT_TRUE(ps.size() == 2);
    EXPECT_TRUE(close_point(ps[0], FP(-4, 3)));
    EXPECT_TRUE(close_point(ps[1], FP(4, 3)));

    auto tangent = c.line_intersections(IL(IP(-10, 5), IP(10, 5)));
    EXPECT_TRUE(tangent.size() == 1);
    EXPECT_TRUE(close_point(tangent[0], FP(0, 5)));
    EXPECT_TRUE(c.relation_to_line(IL(IP(-10, 6), IP(10, 6))) == LineCircleRelation::Separate);

    auto chord = c.segment_intersections(IL(IP(-10, 0), IP(10, 0)));
    EXPECT_TRUE(chord.size() == 2);
    EXPECT_TRUE(close_point(chord[0], FP(-5, 0)));
    EXPECT_TRUE(close_point(chord[1], FP(5, 0)));

    auto half = c.segment_intersections(IL(IP(-10, 0), IP(0, 0)));
    EXPECT_TRUE(half.size() == 1);
    EXPECT_TRUE(close_point(half[0], FP(-5, 0)));

    auto miss = c.segment_intersections(IL(IP(-10, 0), IP(-6, 0)));
    EXPECT_TRUE(miss.empty());
    EXPECT_TRUE(close(c.length_inside_segment(IL(IP(-10, 0), IP(10, 0))), 10));
    EXPECT_TRUE(close(c.length_inside_segment(IL(IP(-10, 0), IP(0, 0))), 5));
}

TEST(GeometryCircle, CircleCircleIntersectionsTangentsAndAreas) {
    IC a(IP(0, 0), 5), b(IP(6, 0), 5);
    auto ps = a.circle_intersections(b);
    EXPECT_TRUE(ps.size() == 2);
    EXPECT_TRUE(close(ps[0].x, 3));
    EXPECT_TRUE(close(std::abs((double)ps[0].y), 4));
    EXPECT_TRUE(close(ps[1].x, 3));
    EXPECT_TRUE(close(std::abs((double)ps[1].y), 4));

    auto tangent = a.circle_intersections(IC(IP(10, 0), 5));
    EXPECT_TRUE(tangent.size() == 1);
    EXPECT_TRUE(close_point(tangent[0], FP(5, 0)));

    auto tp = IC(IP(0, 0), 3).tangent_points(IP(5, 0));
    EXPECT_TRUE(tp.size() == 2);
    EXPECT_TRUE(close(tp[0].x, 1.8L));
    EXPECT_TRUE(close(std::abs((double)tp[0].y), 2.4L));
    EXPECT_TRUE(close(tp[1].x, 1.8L));
    EXPECT_TRUE(close(std::abs((double)tp[1].y), 2.4L));

    OY::GEO::real_type pi = std::acos((OY::GEO::real_type)-1);
    RC u(FP(0, 0), 1), v(FP(1, 0), 1);
    EXPECT_TRUE(close(u.intersection_area(v), 2 * pi / 3 - std::sqrt((OY::GEO::real_type)3) / 2));
    EXPECT_TRUE(close(u.intersection_area(RC(FP(0, 0), 1)), pi));
    EXPECT_TRUE(close(IC(IP(0, 0), 5).intersection_area(IC(IP(0, 0), 2)), 4 * pi));
}

TEST(GeometryCircle, CircleConstructionAndMinimumEnclosingCircle) {
    auto d = OY::GEO::circle_from_diameter(IP(0, 0), IP(2, 0));
    EXPECT_TRUE(close_point(d.o, FP(1, 0)));
    EXPECT_TRUE(close(d.r, 1));

    auto cc = OY::GEO::circumcircle(IP(0, 0), IP(0, 2), IP(2, 0));
    EXPECT_TRUE(close_point(cc.o, FP(1, 1)));
    EXPECT_TRUE(close(cc.r, std::sqrt((OY::GEO::real_type)2)));

    auto mec = OY::GEO::minimum_enclosing_circle(std::vector<IP>{
        IP(0, 0), IP(2, 0), IP(0, 2), IP(2, 2)
    });
    EXPECT_TRUE(close_point(mec.o, FP(1, 1)));
    EXPECT_TRUE(close(mec.r, std::sqrt((OY::GEO::real_type)2)));

    auto random_mec = OY::GEO::minimum_enclosing_circle(std::vector<IP>{
        IP(0, 0), IP(2, 0), IP(0, 2), IP(2, 2)
    }, true);
    EXPECT_TRUE(close_point(random_mec.o, FP(1, 1)));
    EXPECT_TRUE(close(random_mec.r, std::sqrt((OY::GEO::real_type)2)));

    auto collinear = OY::GEO::minimum_enclosing_circle(std::vector<IP>{
        IP(1, 0), IP(0, 0), IP(3, 0)
    });
    EXPECT_TRUE(close_point(collinear.o, FP(1.5L, 0)));
    EXPECT_TRUE(close(collinear.r, 1.5L));

    auto obtuse = OY::GEO::minimum_enclosing_circle(std::vector<IP>{
        IP(0, 0), IP(4, 0), IP(1, 1)
    });
    EXPECT_TRUE(close_point(obtuse.o, FP(2, 0)));
    EXPECT_TRUE(close(obtuse.r, 2));

    std::vector<IP> tri{IP(0, 0), IP(1, 1), IP(4, 0)};
    do {
        auto c = OY::GEO::minimum_enclosing_circle(tri);
        EXPECT_TRUE(close_point(c.o, FP(2, 0)));
        EXPECT_TRUE(close(c.r, 2));
    } while (std::next_permutation(tri.begin(), tri.end()));
}

TEST(GeometryCircle, CircleWithPolygonAreaAndBoundaryLength) {
    OY::GEO::real_type pi = std::acos((OY::GEO::real_type)-1);
    Poly square{IP(-2, -2), IP(2, -2), IP(2, 2), IP(-2, 2)};
    Poly square_cw{IP(-2, -2), IP(-2, 2), IP(2, 2), IP(2, -2)};
    IC unit(IP(0, 0), 1);
    IC big(IP(0, 0), 10);

    EXPECT_TRUE(close(unit.intersection_area(square), pi));
    EXPECT_TRUE(close(unit.intersection_area(square_cw), pi));
    EXPECT_TRUE(close(big.intersection_area(square), 16));
    EXPECT_TRUE(close(big.intersection_area(square_cw), 16));
    EXPECT_TRUE(close(unit.length_inside_polygon(square), 0));
    EXPECT_TRUE(close(big.length_inside_polygon(square), 16));

    Poly half{IP(0, -2), IP(2, -2), IP(2, 2), IP(0, 2)};
    EXPECT_TRUE(close(unit.intersection_area(half), pi / 2));
    EXPECT_TRUE(close(unit.length_inside_polygon(half), 2));

    Poly quadrant{IP(0, 0), IP(2, 0), IP(2, 2), IP(0, 2)};
    EXPECT_TRUE(close(unit.intersection_area(quadrant), pi / 4));
    EXPECT_TRUE(close(unit.length_inside_polygon(quadrant), 2));

    Poly tri{IP(0, 0), IP(3, 0), IP(0, 4)};
    EXPECT_TRUE(close(big.intersection_area(tri), 6));
    EXPECT_TRUE(close(unit.intersection_area(Poly{IP(2, 2), IP(3, 2), IP(2, 3)}), 0));

    IC circle(IP(0, 0), 5);
    EXPECT_TRUE(close(circle.arc_length_inside(IC(IP(0, 0), 10)), circle.perimeter()));
    EXPECT_TRUE(close(circle.arc_length_inside(IC(IP(0, 0), 3)), 0));
    EXPECT_TRUE(close(circle.arc_length_inside(IC(IP(5, 0), 5)), 10 * pi / 3));
}

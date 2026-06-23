#include "src/Geometry/Polygon.h"
#include <gtest/gtest.h>

using IP = OY::GEO::Point<int>;
using RP = OY::GEO::Point<OY::GEO::real_type>;
using RL = OY::GEO::Line<OY::GEO::real_type>;
using Poly = OY::GEO::Polygon<int>;

TEST(GeometryPolygon, PolygonBasicMeasurementsAndPointRelation) {
    Poly rect{IP(0, 0), IP(4, 0), IP(4, 3), IP(0, 3)};
    EXPECT_TRUE(rect.size() == 4);
    EXPECT_TRUE(rect.signed_area2() == 24);
    EXPECT_TRUE(std::abs((double)rect.area() - 12.0) < 1e-12);
    EXPECT_TRUE(std::abs((double)rect.perimeter() - 14.0) < 1e-12);

    auto c = rect.centroid();
    EXPECT_TRUE(std::abs((double)c.x - 2.0) < 1e-12);
    EXPECT_TRUE(std::abs((double)c.y - 1.5) < 1e-12);

    EXPECT_TRUE(rect.relation(IP(2, 1)) == 1);
    EXPECT_TRUE(rect.relation(IP(4, 2)) == 2);
    EXPECT_TRUE(rect.relation(IP(5, 2)) == 0);
    EXPECT_TRUE(rect.contains(IP(4, 2)));
    EXPECT_TRUE(!rect.contains(IP(4, 2), false));
    EXPECT_TRUE(rect.contains_segment(OY::GEO::Line<int>(IP(1, 1), IP(3, 2))));
    EXPECT_TRUE(!rect.contains_segment(OY::GEO::Line<int>(IP(1, 1), IP(5, 2))));
    EXPECT_TRUE(rect.contains_segment(OY::GEO::Line<int>(IP(0, 0), IP(4, 0))));
    EXPECT_TRUE(!rect.contains_segment(OY::GEO::Line<int>(IP(0, 0), IP(4, 0)), false));

    EXPECT_TRUE(rect.boundary_lattice_points() == 14);
    EXPECT_TRUE(rect.interior_lattice_points() == 6);
}

TEST(GeometryPolygon, PolygonConvexityHandlesCollinearPoints) {
    Poly with_collinear{IP(0, 0), IP(2, 0), IP(4, 0), IP(4, 3), IP(0, 3)};
    EXPECT_TRUE(with_collinear.is_convex());
    EXPECT_TRUE(!with_collinear.is_convex(false));

    Poly concave{IP(0, 0), IP(4, 0), IP(2, 1), IP(4, 4), IP(0, 4)};
    EXPECT_TRUE(!concave.is_convex());
    EXPECT_TRUE(concave.relation(IP(1, 1)) == 1);
    EXPECT_TRUE(concave.relation(IP(3, 2)) == 0);

    Poly u_shape{
        IP(0, 0), IP(4, 0), IP(4, 4), IP(3, 4),
        IP(3, 1), IP(1, 1), IP(1, 4), IP(0, 4)
    };
    EXPECT_TRUE(u_shape.contains(RP(0.5L, 2)));
    EXPECT_TRUE(u_shape.contains(RP(3.5L, 2)));
    EXPECT_TRUE(!u_shape.contains(RP(2, 2)));
    EXPECT_TRUE(!u_shape.contains_segment(RL(RP(0.5L, 2), RP(3.5L, 2))));
    EXPECT_TRUE(u_shape.contains_segment(OY::GEO::Line<int>(IP(1, 1), IP(3, 1))));
}

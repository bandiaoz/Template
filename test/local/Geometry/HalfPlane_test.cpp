#include "src/Geometry/HalfPlane.h"
#include <gtest/gtest.h>

using IP = OY::GEO::Point<int>;
using Poly = OY::GEO::Polygon<int>;
using HP = OY::GEO::HalfPlane<int>;

static bool close_to(OY::GEO::real_type a, OY::GEO::real_type b) {
    return std::abs((double)(a - b)) < 1e-10;
}

TEST(GeometryHalfPlane, HalfPlaneKeepsTheLeftSideOfDirectedLine) {
    HP h(IP(0, 0), IP(4, 0));
    EXPECT_TRUE(h.relation(IP(1, 2)) == 1);
    EXPECT_TRUE(h.relation(IP(1, -2)) == -1);
    EXPECT_TRUE(h.contains(IP(2, 0)));
    EXPECT_TRUE(!h.contains(IP(2, 0), false));
    EXPECT_TRUE(h.contains(IP(1, 2)));
    EXPECT_TRUE(!h.contains(IP(1, -2)));
}

TEST(GeometryHalfPlane, CutPolygonClipsByOneHalfPlane) {
    Poly square{IP(-1, -1), IP(1, -1), IP(1, 1), IP(-1, 1)};
    HP keep_right(IP(0, 1), IP(0, -1));

    auto res = OY::GEO::cut_polygon(square, keep_right);
    EXPECT_TRUE(res.size() == 4);
    EXPECT_TRUE(close_to(res.area(), 2));
    for (auto &p : res.p) EXPECT_TRUE(p.x >= -OY::GEO::EPS);
}

TEST(GeometryHalfPlane, HalfPlaneIntersectionHandlesBoundedAndUnboundedCases) {
    std::vector<HP> square{
        HP(IP(-1, -1), IP(1, -1)),
        HP(IP(1, -1), IP(1, 1)),
        HP(IP(1, 1), IP(-1, 1)),
        HP(IP(-1, 1), IP(-1, -1))
    };
    auto res = OY::GEO::half_plane_intersection(square);
    EXPECT_TRUE(res.size() == 4);
    EXPECT_TRUE(close_to(res.area(), 4));

    square.push_back(HP(IP(0, -1), IP(0, 1)));
    res = OY::GEO::half_plane_intersection(square);
    EXPECT_TRUE(res.size() == 4);
    EXPECT_TRUE(close_to(res.area(), 2));
    for (auto &p : res.p) EXPECT_TRUE(p.x <= OY::GEO::EPS);

    std::vector<HP> empty{
        HP(IP(1, 1), IP(1, -1)),
        HP(IP(0, -1), IP(0, 1)),
        HP(IP(-1, -1), IP(2, -1)),
        HP(IP(2, 1), IP(-1, 1))
    };
    EXPECT_TRUE(OY::GEO::half_plane_intersection(empty).empty());

    std::vector<HP> unbounded{
        HP(IP(0, 1), IP(0, 0)),
        HP(IP(0, 0), IP(1, 0)),
        HP(IP(0, 1), IP(1, 0))
    };
    EXPECT_TRUE(OY::GEO::half_plane_intersection(unbounded).empty());

    auto clipped = OY::GEO::half_plane_intersection_with_box(unbounded, 10);
    EXPECT_TRUE(!clipped.empty());
    for (auto &p : clipped.p) {
        EXPECT_TRUE(p.x >= -OY::GEO::EPS);
        EXPECT_TRUE(p.y >= -OY::GEO::EPS);
        EXPECT_TRUE(p.x + p.y >= 1 - OY::GEO::EPS);
    }
}

TEST(GeometryHalfPlane, PolygonKernelAcceptsBothOrientations) {
    Poly ccw{IP(0, 0), IP(4, 0), IP(4, 3), IP(0, 3)};
    Poly cw{IP(0, 0), IP(0, 3), IP(4, 3), IP(4, 0)};

    auto a = OY::GEO::polygon_kernel(ccw);
    auto b = OY::GEO::polygon_kernel(cw);
    EXPECT_TRUE(a.size() == 4);
    EXPECT_TRUE(b.size() == 4);
    EXPECT_TRUE(close_to(a.area(), 12));
    EXPECT_TRUE(close_to(b.area(), 12));
}

TEST(GeometryHalfPlane, ConvexIntersectionReturnsTheOverlapPolygon) {
    Poly a{IP(0, 0), IP(2, 0), IP(2, 2), IP(0, 2)};
    Poly b{IP(1, 1), IP(3, 1), IP(3, 3), IP(1, 3)};

    auto res = OY::GEO::convex_intersection(a, b);
    EXPECT_TRUE(res.size() == 4);
    EXPECT_TRUE(close_to(res.area(), 1));
    for (auto &p : res.p) {
        EXPECT_TRUE(p.x >= 1 - OY::GEO::EPS);
        EXPECT_TRUE(p.x <= 2 + OY::GEO::EPS);
        EXPECT_TRUE(p.y >= 1 - OY::GEO::EPS);
        EXPECT_TRUE(p.y <= 2 + OY::GEO::EPS);
    }
}

#include "src/Geometry/Convex.h"
#include <gtest/gtest.h>

using IP = OY::GEO::Point<int>;
using FP = OY::GEO::Point<OY::GEO::real_type>;
using Hull = OY::GEO::Convex<int>;
using FHull = OY::GEO::Convex<OY::GEO::real_type>;

TEST(GeometryConvex, ConvexHullConstructionAndContainment) {
    std::vector<IP> points{
        IP(0, 0), IP(2, 0), IP(2, 2), IP(0, 2),
        IP(1, 1), IP(1, 0), IP(0, 2)
    };
    Hull h(points);
    EXPECT_TRUE(h.size() == 4);
    EXPECT_TRUE(h[0] == IP(0, 0));
    EXPECT_TRUE(h[1] == IP(2, 0));
    EXPECT_TRUE(h[2] == IP(2, 2));
    EXPECT_TRUE(h[3] == IP(0, 2));

    EXPECT_TRUE(h.relation(IP(1, 1)) == 1);
    EXPECT_TRUE(h.relation(IP(2, 1)) == 2);
    EXPECT_TRUE(h.relation(IP(3, 1)) == 0);
    EXPECT_TRUE(h.contains(IP(2, 1)));
    EXPECT_TRUE(!h.contains(IP(2, 1), false));

    FHull single{FP(0, 0)};
    EXPECT_TRUE(single.relation(FP(0, 0)) == 2);
    EXPECT_TRUE(single.relation(FP(1e-13L, 0)) == 2);
    EXPECT_TRUE(single.relation(FP(1e-9L, 0)) == 0);

    Hull seg(std::vector<IP>{IP(0, 0), IP(1, 0), IP(2, 0)});
    EXPECT_TRUE(seg.size() == 2);
    EXPECT_TRUE(seg.relation(IP(1, 0)) == 2);
    EXPECT_TRUE(seg.relation(IP(3, 0)) == 0);

    auto keep_collinear = Hull::convex_hull(points, true);
    EXPECT_TRUE(keep_collinear.size() == 5);

    Hull line(std::vector<IP>{IP(0, 0), IP(1, 0), IP(2, 0)});
    auto [line_d, line_a, line_b] = line.diameter();
    EXPECT_TRUE(std::abs((double)line_d - 2.0) < 1e-12);
    EXPECT_TRUE(OY::GEO::distance(line[line_a], line[line_b]) == 2);
}

TEST(GeometryConvex, ConvexHelpersCoverLineIntersectionUniquenessAndDiameter) {
    std::vector<IP> unique_points{
        IP(0, 0), IP(2, 0), IP(4, 0), IP(4, 2), IP(4, 4),
        IP(2, 4), IP(0, 4), IP(0, 2)
    };
    Hull h(unique_points);
    EXPECT_TRUE(h.is_unique(unique_points));
    EXPECT_TRUE(!h.is_unique(std::vector<IP>{IP(0, 0), IP(4, 0), IP(4, 4), IP(0, 4)}));

    EXPECT_TRUE(h.line_intersects(OY::GEO::Line<int>(IP(2, -1), IP(2, 5))));
    EXPECT_TRUE(!h.line_intersects(OY::GEO::Line<int>(IP(5, 0), IP(5, 4))));
    EXPECT_TRUE(h.line_intersects(OY::GEO::Line<int>(IP(2, 2), IP(2, 2))));
    EXPECT_TRUE(!h.line_intersects(OY::GEO::Line<int>(IP(5, 5), IP(5, 5))));

    auto [d, a, b] = h.diameter();
    EXPECT_TRUE(std::abs((double)d - std::sqrt(32.0)) < 1e-12);
    EXPECT_TRUE(std::abs((double)OY::GEO::distance(h[a], h[b]) - std::sqrt(32.0)) < 1e-12);
}

TEST(GeometryConvex, ConvexRelationAndDiameterMatchBruteForceOnSmallSets) {
    unsigned seed = 123456789;
    auto rnd = [&]() {
        seed = seed * 1103515245u + 12345u;
        return (seed >> 16) & 32767u;
    };

    for (int tc = 0; tc < 300; tc++) {
        int m = 1 + rnd() % 12;
        std::vector<IP> points;
        for (int i = 0; i < m; i++) {
            points.push_back(IP((int)(rnd() % 9) - 4, (int)(rnd() % 9) - 4));
        }
        Hull h(points);
        OY::GEO::Polygon<int> poly(h.p);

        for (int x = -5; x <= 5; x++) {
            for (int y = -5; y <= 5; y++) {
                IP q(x, y);
                EXPECT_TRUE(h.relation(q) == poly.relation(q));
            }
        }

        long long best2 = 0;
        for (int i = 0; i < h.size(); i++) {
            for (int j = i + 1; j < h.size(); j++) {
                long long d2 = (long long)(h[j] - h[i]).norm2();
                best2 = std::max(best2, d2);
            }
        }
        auto [d, a, b] = h.diameter();
        EXPECT_TRUE(std::abs((double)d - std::sqrt((double)best2)) < 1e-12);
        EXPECT_TRUE(0 <= a);
        EXPECT_TRUE(a < h.size());
        EXPECT_TRUE(0 <= b);
        EXPECT_TRUE(b < h.size());
    }
}

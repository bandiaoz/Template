#include "src/Geometry/Convex.h"
#include "third_party/catch/catch_amalgamated.hpp"

using IP = OY::GEO::Point<int>;
using FP = OY::GEO::Point<OY::GEO::real_type>;
using Hull = OY::GEO::Convex<int>;
using FHull = OY::GEO::Convex<OY::GEO::real_type>;

TEST_CASE("Convex hull construction and containment", "[Geometry][Convex]") {
    std::vector<IP> points{
        IP(0, 0), IP(2, 0), IP(2, 2), IP(0, 2),
        IP(1, 1), IP(1, 0), IP(0, 2)
    };
    Hull h(points);
    REQUIRE(h.size() == 4);
    REQUIRE(h[0] == IP(0, 0));
    REQUIRE(h[1] == IP(2, 0));
    REQUIRE(h[2] == IP(2, 2));
    REQUIRE(h[3] == IP(0, 2));

    REQUIRE(h.relation(IP(1, 1)) == 1);
    REQUIRE(h.relation(IP(2, 1)) == 2);
    REQUIRE(h.relation(IP(3, 1)) == 0);
    REQUIRE(h.contains(IP(2, 1)));
    REQUIRE(!h.contains(IP(2, 1), false));

    FHull single{FP(0, 0)};
    REQUIRE(single.relation(FP(0, 0)) == 2);
    REQUIRE(single.relation(FP(1e-13L, 0)) == 2);
    REQUIRE(single.relation(FP(1e-9L, 0)) == 0);

    Hull seg(std::vector<IP>{IP(0, 0), IP(1, 0), IP(2, 0)});
    REQUIRE(seg.size() == 2);
    REQUIRE(seg.relation(IP(1, 0)) == 2);
    REQUIRE(seg.relation(IP(3, 0)) == 0);

    auto keep_collinear = Hull::convex_hull(points, true);
    REQUIRE(keep_collinear.size() == 5);

    Hull line(std::vector<IP>{IP(0, 0), IP(1, 0), IP(2, 0)});
    auto [line_d, line_a, line_b] = line.diameter();
    REQUIRE(std::abs((double)line_d - 2.0) < 1e-12);
    REQUIRE(OY::GEO::distance(line[line_a], line[line_b]) == 2);
}

TEST_CASE("Convex helpers cover line intersection uniqueness and diameter", "[Geometry][Convex]") {
    std::vector<IP> unique_points{
        IP(0, 0), IP(2, 0), IP(4, 0), IP(4, 2), IP(4, 4),
        IP(2, 4), IP(0, 4), IP(0, 2)
    };
    Hull h(unique_points);
    REQUIRE(h.is_unique(unique_points));
    REQUIRE(!h.is_unique(std::vector<IP>{IP(0, 0), IP(4, 0), IP(4, 4), IP(0, 4)}));

    REQUIRE(h.line_intersects(OY::GEO::Line<int>(IP(2, -1), IP(2, 5))));
    REQUIRE(!h.line_intersects(OY::GEO::Line<int>(IP(5, 0), IP(5, 4))));
    REQUIRE(h.line_intersects(OY::GEO::Line<int>(IP(2, 2), IP(2, 2))));
    REQUIRE(!h.line_intersects(OY::GEO::Line<int>(IP(5, 5), IP(5, 5))));

    auto [d, a, b] = h.diameter();
    REQUIRE(std::abs((double)d - std::sqrt(32.0)) < 1e-12);
    REQUIRE(std::abs((double)OY::GEO::distance(h[a], h[b]) - std::sqrt(32.0)) < 1e-12);
}

TEST_CASE("Convex relation and diameter match brute force on small sets", "[Geometry][Convex]") {
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
                REQUIRE(h.relation(q) == poly.relation(q));
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
        REQUIRE(std::abs((double)d - std::sqrt((double)best2)) < 1e-12);
        REQUIRE(0 <= a);
        REQUIRE(a < h.size());
        REQUIRE(0 <= b);
        REQUIRE(b < h.size());
    }
}

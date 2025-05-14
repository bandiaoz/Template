#include "src/String/SequenceHash.h"
#include "src/Math/Modular/StaticModInt64.h"
#include "third_party/catch/catch_amalgamated.hpp"

using hash_mod = OY::mint4611686018427387847;
using table_t = OY::SEQHASH::SeqHashPresumTable<hash_mod>;
using hash_t = table_t::hash_type;

// 添加一个全局初始化器，在所有测试运行前执行初始化
namespace {
    struct SeqHashInitializer {
        SeqHashInitializer() { 
            // 设置随机 base，预处理
            hash_t::s_info.set_random_base_and_prepare(1000000);
        }
    };
    static SeqHashInitializer seqHashInitializerInstance;
}

TEST_CASE("hash_type construction", "[SequenceHash]") {
    std::string s = "abcdefg";
    hash_t h(s);
    REQUIRE(h.m_len == s.size());
}

TEST_CASE("hash_type functions", "[SequenceHash]") {
    std::string s1 = "abcd", s2 = "efg", s3 = "abcdefg";
    hash_t h1(s1), h2(s2), h3(s3);
    REQUIRE(s1 + s2 == s3);
    REQUIRE(h1.append_right(h2) == h3);

    // hash_t 本身是可以进行拼接的
    // 需要使用 append_right 和 append_left 函数，而不是直接使用 + 运算符!!
    SECTION("append_right") {
        hash_t result = h1.append_right(h2); 
        REQUIRE(result.m_len == s1.size() + s2.size());
        REQUIRE(result == h3);
    }
    SECTION("append_left") {
        hash_t result = h2.append_left(h1); 
        REQUIRE(result.m_len == s1.size() + s2.size());
        REQUIRE(result == h3);
    }

    // 删除一个前缀
    SECTION("remove_prefix") {
        hash_t result = h3.remove_prefix(h1);
        REQUIRE(result.m_len == s2.size());
        REQUIRE(result == h2);
    }
    // 删除一个后缀
    SECTION("remove_suffix") {
        hash_t result = h3.remove_suffix(h2);
        REQUIRE(result.m_len == s1.size());
        REQUIRE(result == h1);
    }

    // 重复 s3 三次
    SECTION("repeat_for") {
        hash_t result = h3.repeat_for(3);
        REQUIRE(result.m_len == s3.size() * 3);
        REQUIRE(result == h3.append_right(h3).append_right(h3));
    }
}

TEST_CASE("hash_type operators", "[SequenceHash]") {
    std::string s1 = "aaa", s2 = "bb";
    hash_t h1(s1), h2(s2);

    // 如果 m_len 和 m_val 都相同，则认为两个 hash_t 相等
    SECTION("== and !=") {
        REQUIRE(h1 == h1);
        REQUIRE(!(h1 != h1));
        REQUIRE(h1 != h2);
        REQUIRE(!(h1 == h2));
    }

    // 比较大小，按照 m_len, m_val 的顺序比较
    SECTION("compare") {
        REQUIRE(h1 > h2);
        REQUIRE(h1 >= h2);
        REQUIRE(h2 < h1);
        REQUIRE(h2 <= h1);
    }

    // m_len 相同的 hash_t 之间可以进行 + 和 - 运算
    SECTION("+ and -") {
        std::string s1 = std::string(4, 'a');
        std::string s2 = std::string(4, 'b');
        hash_t h1(s1), h2(s2);

        hash_t hash_sum = h1 + h2;
        REQUIRE(hash_sum.m_len == h1.m_len);
        REQUIRE(hash_sum.m_val == h1.m_val + h2.m_val);

        hash_t hash_diff = hash_sum - h1;
        REQUIRE(hash_diff == h2);
    }

    // 大写字母 + 32 变成小写字母
    // + 运算相当于对两个长度相等的 hash_t 逐位相加
    SECTION("+ operator") {
        std::string s_upper(4, 'A');
        std::string s_lower(4, 'a');
        hash_t h_upper(s_upper), h_lower(s_lower);

        std::vector<int> inc(4, 32);
        hash_t hash_sum = h_upper + hash_t(inc);

        REQUIRE(hash_sum.m_len == h_lower.m_len);
        REQUIRE(hash_sum.m_val == h_lower.m_val);
    }
}

TEST_CASE("table_t construction", "[SequenceHash]") {
    const int n = 26;
    std::string s = "abcdefghijklmnopqrstuvwxyz";
    std::vector<char> t(s.begin(), s.end());

    auto check = [&](const table_t &table, const std::string &s) {
        hash_t hash_all = table.query_hash(0, s.size());
        REQUIRE(hash_all.m_len == s.size());
        REQUIRE(hash_all == hash_t(s));
    };
    table_t table1(n, [&](int32_t i) { return s[i]; });
    table_t table2(s.begin(), s.end());
    table_t table3(s);
    table_t table4(t);
    check(table1, s);
    check(table2, s);
    check(table3, s);
    check(table4, s);
}

TEST_CASE("table_t functions", "[SequenceHash]") {
    // 查询区间的哈希结果，允许区间为空
    SECTION("query_hash") {
        std::string s = "abcdefghijklmnopqrstuvwxyz";
        table_t table(s);
        auto check = [&](size_t l, size_t r) {
            size_t len = r - l;
            hash_t h = table.query_hash(l, r);
            REQUIRE(h.m_len == len);
            REQUIRE(h == hash_t(s.substr(l, len)));
        };

        check(2, 5);
        check(0, s.size());
        check(s.size(), s.size());
    }

    // 查询两个串的任意长度子串的 lcp
    SECTION("lcp") {
        std::string s1 = "aaaaaaaa", s2 = "aaaabbbb";
        table_t table1(s1), table2(s2);
        int start1 = 0, start2 = 0;
        // lcp(s1.substr(0), s2.substr(0)) == 4
        REQUIRE(table_t::lcp(table1, start1, table2, start2) == 4);
        // lcp(s1.substr(0, 3), s2.substr(0, 3)) == 3
        REQUIRE(table_t::lcp(table1, start1, table2, start2, 3) == 3);
    }
}


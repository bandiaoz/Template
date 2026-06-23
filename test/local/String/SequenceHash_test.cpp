#include "src/String/SequenceHash.h"
#include "src/Math/Modular/StaticModInt64.h"
#include <gtest/gtest.h>

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

TEST(SequenceHash, HashTypeConstruction) {
    std::string s = "abcdefg";
    hash_t h(s);
    EXPECT_TRUE(h.m_len == s.size());
}

TEST(SequenceHash, HashTypeFunctions) {
    std::string s1 = "abcd", s2 = "efg", s3 = "abcdefg";
    hash_t h1(s1), h2(s2), h3(s3);
    EXPECT_TRUE(s1 + s2 == s3);
    EXPECT_TRUE(h1.append_right(h2) == h3);

    // hash_t 本身是可以进行拼接的
    // 需要使用 append_right 和 append_left 函数，而不是直接使用 + 运算符!!
    {
        hash_t result = h1.append_right(h2); 
        EXPECT_TRUE(result.m_len == s1.size() + s2.size());
        EXPECT_TRUE(result == h3);
    }
    {
        hash_t result = h2.append_left(h1); 
        EXPECT_TRUE(result.m_len == s1.size() + s2.size());
        EXPECT_TRUE(result == h3);
    }

    // 删除一个前缀
    {
        hash_t result = h3.remove_prefix(h1);
        EXPECT_TRUE(result.m_len == s2.size());
        EXPECT_TRUE(result == h2);
    }
    // 删除一个后缀
    {
        hash_t result = h3.remove_suffix(h2);
        EXPECT_TRUE(result.m_len == s1.size());
        EXPECT_TRUE(result == h1);
    }

    // 重复 s3 三次
    {
        hash_t result = h3.repeat_for(3);
        EXPECT_TRUE(result.m_len == s3.size() * 3);
        EXPECT_TRUE(result == h3.append_right(h3).append_right(h3));
    }
}

TEST(SequenceHash, HashTypeOperators) {
    std::string s1 = "aaa", s2 = "bb";
    hash_t h1(s1), h2(s2);

    // 如果 m_len 和 m_val 都相同，则认为两个 hash_t 相等
    {
        EXPECT_TRUE(h1 == h1);
        EXPECT_TRUE(!(h1 != h1));
        EXPECT_TRUE(h1 != h2);
        EXPECT_TRUE(!(h1 == h2));
    }

    // 比较大小，按照 m_len, m_val 的顺序比较
    {
        EXPECT_TRUE(h1 > h2);
        EXPECT_TRUE(h1 >= h2);
        EXPECT_TRUE(h2 < h1);
        EXPECT_TRUE(h2 <= h1);
    }

    // m_len 相同的 hash_t 之间可以进行 + 和 - 运算
    {
        std::string s1 = std::string(4, 'a');
        std::string s2 = std::string(4, 'b');
        hash_t h1(s1), h2(s2);

        hash_t hash_sum = h1 + h2;
        EXPECT_TRUE(hash_sum.m_len == h1.m_len);
        EXPECT_TRUE(hash_sum.m_val == h1.m_val + h2.m_val);

        hash_t hash_diff = hash_sum - h1;
        EXPECT_TRUE(hash_diff == h2);
    }

    // 大写字母 + 32 变成小写字母
    // + 运算相当于对两个长度相等的 hash_t 逐位相加
    {
        std::string s_upper(4, 'A');
        std::string s_lower(4, 'a');
        hash_t h_upper(s_upper), h_lower(s_lower);

        std::vector<int> inc(4, 32);
        hash_t hash_sum = h_upper + hash_t(inc);

        EXPECT_TRUE(hash_sum.m_len == h_lower.m_len);
        EXPECT_TRUE(hash_sum.m_val == h_lower.m_val);
    }
}

TEST(SequenceHash, TableTConstruction) {
    const int n = 26;
    std::string s = "abcdefghijklmnopqrstuvwxyz";
    std::vector<char> t(s.begin(), s.end());

    auto check = [&](const table_t &table, const std::string &s) {
        hash_t hash_all = table.query_hash(0, s.size());
        EXPECT_TRUE(hash_all.m_len == s.size());
        EXPECT_TRUE(hash_all == hash_t(s));
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

TEST(SequenceHash, TableTFunctions) {
    // 查询区间的哈希结果，允许区间为空
    {
        std::string s = "abcdefghijklmnopqrstuvwxyz";
        table_t table(s);
        auto check = [&](size_t l, size_t r) {
            size_t len = r - l;
            hash_t h = table.query_hash(l, r);
            EXPECT_TRUE(h.m_len == len);
            EXPECT_TRUE(h == hash_t(s.substr(l, len)));
        };

        check(2, 5);
        check(0, s.size());
        check(s.size(), s.size());
    }

    // 查询两个串的任意长度子串的 lcp
    {
        std::string s1 = "aaaaaaaa", s2 = "aaaabbbb";
        table_t table1(s1), table2(s2);
        int start1 = 0, start2 = 0;
        // lcp(s1.substr(0), s2.substr(0)) == 4
        EXPECT_TRUE(table_t::lcp(table1, start1, table2, start2) == 4);
        // lcp(s1.substr(0, 3), s2.substr(0, 3)) == 3
        EXPECT_TRUE(table_t::lcp(table1, start1, table2, start2, 3) == 3);
    }
}


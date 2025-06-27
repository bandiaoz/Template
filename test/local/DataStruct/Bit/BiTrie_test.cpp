#include "src/DataStruct/Bit/BiTrie.h"
#include <algorithm>
#include "third_party/catch/catch_amalgamated.hpp"

TEST_CASE("BiTrie::Tree constructor", "[BiTrie]") {
    SECTION("uint32_t constructor") {
        constexpr int L = 32;
        using Trie = OY::BiTrie::Tree<uint32_t, L>;
        // 可处理范围为 $[0, 2^L - 1]$ 的数字
        Trie trie;
        REQUIRE(trie.empty());
    }
    SECTION("uint64_t constructor") {
        constexpr int L = 64;
        using Trie = OY::BiTrie::Tree<uint64_t, L>;
        Trie trie;
        REQUIRE(trie.empty());
    }
    SECTION("uint32_t constructor with info") {
        constexpr int L = 32;
        struct Info {};
        using Trie = OY::BiTrie::Tree<uint32_t, L, Info>;
        Trie trie;
        REQUIRE(trie.empty());
    }
}

TEST_CASE("BiTrie::Tree insert and contains", "[BiTrie]") {
    using Trie = OY::BiTrie::Tree<uint32_t, 5>;
    Trie trie;
    // 空树
    REQUIRE(trie.empty());

    std::vector<uint32_t> numbers = {4, 6, 9, 2, 3, 5, 4, 4, 1, 5};
    for (uint32_t x : numbers) {
        trie.insert(x);
    }

    // trie 非空
    REQUIRE(!trie.empty());
    // 查询是否包含某元素
    REQUIRE(!trie.contains(3)->is_null());
    REQUIRE(trie.contains(10)->is_null());
    // 查询与 8 的最大异或，返回适配的叶子结点指针和异或得到的最大值
    REQUIRE(trie.max_bitxor(8).second == 14);
    // 查询与 6 的最小异或，返回适配的叶子结点指针和异或得到的最小值
    REQUIRE(trie.min_bitxor(6).second == 0);
}

TEST_CASE("BiTrie::Tree with Info", "[BiTrie]") {
    // 维护最大出现次数
    struct Info {
        int m_max_cnt;
    };
    using Trie = OY::BiTrie::Tree<uint32_t, 5, Info>;

    Trie trie;
    std::vector<uint32_t> numbers = {4, 6, 9, 2, 3, 5, 4, 4, 1, 5};
    for (uint32_t x : numbers) {
        auto ptr = trie.insert(x);
        ptr->m_max_cnt++;
        int cnt = ptr->m_max_cnt;
        trie.trace(x, [&](Trie::node *p) { p->m_max_cnt = std::max(p->m_max_cnt, cnt); });
    }

    // 出现至少一次的元素中，与 6 的最大异或为：6 xor 9 = 15
    REQUIRE(trie.max_bitxor(6, [&](Trie::node *p) {
        return p->m_max_cnt >= 1;
    }).second == 15);

    // 出现至少两次的元素中，与 6 的最大异或为：6 xor 5 = 3
    REQUIRE(trie.max_bitxor(6, [&](Trie::node *p) {
        return p->m_max_cnt >= 2;
    }).second == 3);

    // 出现至少三次的元素中，与 6 的最大异或为：6 xor 4 = 2
    REQUIRE(trie.max_bitxor(6, [&](Trie::node *p) {
        return p->m_max_cnt >= 3;
    }).second == 2);
}

TEST_CASE("BiTrie::CountTree", "[BiTrie]") {
    using Trie = OY::BiTrie::CountTree<uint32_t, int, 5>;

    Trie trie;
    std::vector<uint32_t> numbers = {4, 6, 9, 2, 3, 5, 4, 4, 1, 5};
    for (uint32_t x : numbers) {
        trie.insert_one(x);
    }

    // 查询与 6 的最小异或
    REQUIRE(trie.min_bitxor(6).second == 0);
    // 查询与 6 的最大异或
    REQUIRE(trie.max_bitxor(6).second == 15);
    // 可以求出第 k 大异或
    // 注意，此处的 k 大异或是不去重的
    std::vector<int> kth_xor;
    for (int k = 0; k < trie.root()->count(); k++) {
        auto res = trie.kth_bitxor(6, k).second;
        kth_xor.push_back(res);
    }
    // 计算所有数与 6 的异或，并排序
    std::vector<int> kth_xor_correct;
    for (uint32_t x : numbers) {
        kth_xor_correct.push_back(6 ^ x);
    }
    std::sort(kth_xor_correct.begin(), kth_xor_correct.end());
    REQUIRE(kth_xor == kth_xor_correct);

    // x 和 6 的异或值是 res 的，查询 res 是第几大异或（0 表示最大的）
    for (int res = 0; res <= trie._mask(); res++) {
        if (int x = res ^ 6; !trie.contains(x)->is_null()) {
            auto rank = trie.rank_bitxor(6, res);
            // 计算第几大：0 表示最大的
            // 等价于所有的异或可能中，有多少个数比 res 大
            int correct_rank = std::distance(std::upper_bound(kth_xor_correct.begin(), kth_xor_correct.end(), res), kth_xor_correct.end());
            REQUIRE(rank == correct_rank);
        }
    }
} 
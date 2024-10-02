#pragma once

#include "src/DataStruct/Segtree/PersistentCompressedTree.h"

/**
 * @brief 区间 mex 查询表
 */
namespace OY {
    template <template <typename> typename BufferType = VectorBufferWithoutCollect>
    class RangeMex {
        using size_type = uint32_t;
        using group = PerCPTREE::BaseMonoid<size_type, 0xffffffff, PerCPTREE::ChoiceByCompare<size_type, std::greater<size_type>>>;
        using tree_type = PerCPTREE::Tree<group, false, size_type, BufferType>;
        std::vector<tree_type> m_trees;
    public:
        static void _reserve(size_type capacity) { tree_type::_reserve(capacity); }
        RangeMex() = default;
        template <typename InitMapping>
        RangeMex(size_type length, InitMapping mapping) { resize(length, mapping); }
        template <typename Iterator>
        RangeMex(Iterator first, Iterator last) { reset(first, last); }
        template <typename InitMapping>
        void resize(size_type length, InitMapping mapping) {
            m_trees.clear();
            m_trees.reserve(length + 1);
            m_trees.emplace_back(length, [&](size_type i) { return 0; });
            for (size_type i = 0; i != length; i++) {
                m_trees.push_back(m_trees.back().copy());
                size_type val = mapping(i);
                if (val < length) m_trees.back().modify(val, i + 1);
            }
        }
        template <typename Iterator>
        void reset(Iterator first, Iterator last) {
            resize(last - first, [&](size_type i) { return *(first + i); });
        }
        /**
         * @brief 查询区间 [left, right) 的 mex
         */
        size_type query(size_type left, size_type right) const {
            right--;
            auto check = [left](size_type v) { return v > left; };
            return std::min<size_type>(m_trees.size() - 1, m_trees[right + 1].max_right(0, check) + 1);
        }
    };
}
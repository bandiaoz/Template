#ifndef __OY_HAMELXORBASETABLE__
#define __OY_HAMELXORBASETABLE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "src/Math/LinearAlgebra/HamelXorBase.h"

/**
 * @brief 前缀异或线性基
 * @example OY::StaticHamelXorBaseTable32<MAX_WIDTH> hxb(length, mapping);
 * @note 构造的复杂度为 $O(n\cdot\omega)$ ，此处 $\omega$ 指线性基的维数。
 * @note 本模板用于加速查询区间线性基。
 */
namespace OY {
    namespace HAMELTABLE {
        using size_type = uint32_t;
        template <typename Tp, size_type MAX_WIDTH>
        struct HamelTableNodes {
            Tp m_val[MAX_WIDTH];
            size_type m_pos[MAX_WIDTH];
            size_type count(size_type left) const {
                return std::count_if(m_pos, m_pos + MAX_WIDTH, [&](size_type pos) { return pos > left; });
            }
        };
        template <typename Tp>
        struct HamelTableNodes<Tp, 0> {
            static size_type s_width;
            std::vector<Tp> m_val = std::vector<Tp>(s_width);
            std::vector<size_type> m_pos = std::vector<size_type>(s_width);
            size_type count(size_type left) const {
                return std::count_if(m_val.begin(), m_val.end(), [&](size_type pos) { return pos > left; });
            }
        };
        template <typename Tp>
        size_type HamelTableNodes<Tp, 0>::s_width = sizeof(Tp) << 3;
        template <typename Tp, size_type MAX_WIDTH>
        struct HamelXorBaseTable {
            using base_type = HAMEL::HamelXorBase<Tp, MAX_WIDTH>;
            std::vector<HamelTableNodes<Tp, MAX_WIDTH>> m_masks;
            static void set_width(size_type w) {
                static_assert(!MAX_WIDTH, "MAX_WIDTH Must Be 0");
                HamelTableNodes<Tp, MAX_WIDTH>::s_width = w;
                base_type::set_width(w);
            }
            static size_type width() {
                if constexpr (MAX_WIDTH)
                    return MAX_WIDTH;
                else
                    return HamelTableNodes<Tp, MAX_WIDTH>::s_width;
            }
            HamelXorBaseTable() { m_masks.push_back({}); }
            template <typename InitMapping>
            HamelXorBaseTable(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            HamelXorBaseTable(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_masks.resize(length + 1);
                for (size_type j = 0; j != width(); j++) m_masks[0].m_val[j] = m_masks[0].m_pos[j] = 0;
                for (size_type i = 0; i != length; i++) {
                    m_masks[i + 1] = m_masks[i];
                    Tp mask = mapping(i);
                    size_type pos = i + 1;
                    for (size_type j = width() - 1; ~j; j--)
                        if (mask >> j & 1) {
                            if (!m_masks[i + 1].m_pos[j]) {
                                m_masks[i + 1].m_val[j] = mask, m_masks[i + 1].m_pos[j] = pos;
                                break;
                            }
                            if (m_masks[i + 1].m_pos[j] <= pos) std::swap(mask, m_masks[i + 1].m_val[j]), std::swap(pos, m_masks[i + 1].m_pos[j]);
                            mask ^= m_masks[i + 1].m_val[j];
                        }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void reserve(size_type length) { m_masks.clear(), m_masks.reserve(length + 1), m_masks.push_back({}); }
            void clear() { m_masks.clear(), m_masks.push_back({}); }
            void push_back(Tp mask) {
                size_type pos = m_masks.size(), i = pos - 1;
                m_masks.push_back(m_masks.back());
                for (size_type j = width() - 1; ~j; j--)
                    if (mask >> j & 1) {
                        if (!m_masks[i + 1].m_pos[j]) {
                            m_masks[i + 1].m_val[j] = mask, m_masks[i + 1].m_pos[j] = pos;
                            break;
                        }
                        if (m_masks[i + 1].m_pos[j] <= pos) std::swap(mask, m_masks[i + 1].m_val[j]), std::swap(pos, m_masks[i + 1].m_pos[j]);
                        mask ^= m_masks[i + 1].m_val[j];
                    }
            }
            void pop_back(Tp mask) { m_masks.pop_back(); }
            /**
             * @brief 从高位到低位顺序枚举区间 [left, right) 中所有基向量
             * @note 复杂度为 $O(\omega)$
             */
            template <typename Callback>
            void enumerate_base(size_type left, size_type right, Callback &&call) const {
                right--;
                auto &node = m_masks[right + 1];
                for (uint32_t i = width() - 1; ~i; i--)
                    if (node.m_pos[i] > left) call(node.m_val[i]);
            }
            /**
             * @brief 将区间 [left, right) 中的所有基向量加入到基础异或线性基中
             * @note 复杂度为 $O(\omega^2)$
             */
            base_type to_base_type(size_type left, size_type right) const {
                base_type res{};
                enumerate_base(left, right, [&](Tp mask) { res.insert(mask); });
                return res;
            }
            /**
             * @brief 查询区间 [left, right) 中所有基向量所能形成的异或线性基的最大异或和
             * @note 复杂度为 $O(\omega)$
             */
            Tp query_max_bitxor(size_type left, size_type right, Tp base = 0) const {
                auto &node = m_masks[right];
                Tp ans = base;
                enumerate_base(left, right, [&](Tp mask) { if ((ans ^ mask) > ans) ans ^= mask; });
                return ans;
            }
        };
    }
    template <HAMELTABLE::size_type MAX_WIDTH, typename = typename std::enable_if<MAX_WIDTH>::type>
    using StaticHamelXorBaseTable32 = HAMELTABLE::HamelXorBaseTable<uint32_t, MAX_WIDTH>;
    template <HAMELTABLE::size_type MAX_WIDTH, typename = typename std::enable_if<MAX_WIDTH>::type>
    using StaticHamelXorBaseTable64 = HAMELTABLE::HamelXorBaseTable<uint64_t, MAX_WIDTH>;
    using DynamicHamelXorBaseTable32 = HAMELTABLE::HamelXorBaseTable<uint32_t, 0>;
    using DynamicHamelXorBaseTable64 = HAMELTABLE::HamelXorBaseTable<uint64_t, 0>;
}

#endif
#pragma once

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "src/Misc/std_bit.h"

/**
 * @brief 异或线性基
 * @example OY::StaticHamelXorBase64<62> hxb{};
 */
namespace OY {
    namespace HAMEL {
        using size_type = uint32_t;
        template <typename Tp, size_type MAX_WIDTH>
        struct MaskNodes {
            Tp m_val[MAX_WIDTH];
            Tp &operator[](size_type index) { return m_val[index]; }
            Tp operator[](size_type index) const { return m_val[index]; }
            size_type count() const {
                return std::count_if(m_val, m_val + MAX_WIDTH, [&](Tp x) { return x; });
            }
        };
        template <typename Tp>
        struct MaskNodes<Tp, 0> {
            static size_type s_width;
            std::vector<Tp> m_val = std::vector<Tp>(s_width);
            Tp &operator[](size_type index) { return m_val[index]; }
            Tp operator[](size_type index) const { return m_val[index]; }
            size_type count() const {
                return std::count_if(m_val.begin(), m_val.end(), [&](Tp x) { return x; });
            }
        };
        template <typename Tp>
        size_type MaskNodes<Tp, 0>::s_width = sizeof(Tp) << 3;
        template <typename Tp, size_type MAX_WIDTH>
        struct HamelXorBase {
            MaskNodes<Tp, MAX_WIDTH> m_masks{};
            static void set_width(size_type w) {
                static_assert(!MAX_WIDTH, "MAX_WIDTH Must Be 0");
                MaskNodes<Tp, MAX_WIDTH>::s_width = w;
            }
            static size_type width() {
                if constexpr (MAX_WIDTH)
                    return MAX_WIDTH;
                else
                    return MaskNodes<Tp, MAX_WIDTH>::s_width;
            }
            template <typename Callback>
            void _dfs(size_type index, Tp mask, const std::vector<size_type> &next, Callback &&call) const {
                size_type i = next[index];
                if (~i)
                    _dfs(i, mask, next, call), _dfs(i, mask ^ m_masks[i], next, call);
                else
                    call(mask);
            }
            HamelXorBase() = default;
            HamelXorBase(Tp mask) : m_masks{} { insert(mask); }
            /**
             * @brief 插入一个向量 `mask`，返回新插入的向量所能贡献的最高位
             */
            size_type insert(Tp mask) {
                for (size_type i = std::bit_width(mask) - 1; mask && ~i; i--) {
                    if (mask >> i & 1) {
                        if (!m_masks[i]) {
                            m_masks[i] = mask;
                            return i;
                        } else {
                            mask ^= m_masks[i];
                        }
                    }
                }
                return -1;
            }
            /**
             * @brief 查询是否可表达向量 `mask`
             */
            bool contains(Tp mask) const {
                for (size_type i = std::bit_width(mask) - 1; mask && ~i; i--)
                    if (m_masks[i] && (mask >> i & 1)) mask ^= m_masks[i];
                return !mask;
            }
            /**
             * @brief 返回基向量的数量
             */
            size_type base_count() const { return m_masks.count(); }
            /**
             * @brief 返回可表达向量的数量，即 `power(2, base_count())`
             */
            Tp total_count() const { return Tp(1) << base_count(); }
            /**
             * @brief 返回可表达的排名为 `k` 的向量
             * @param k 表示排名，`k = 0` 表示最小的向量
             * @param base 表示向量起点
             */
            Tp kth(Tp k, Tp base = 0) const {
                Tp cnt = total_count(), ans = base;
                for (size_type i = width() - 1; cnt > 1 && ~i; i--) {
                    if (m_masks[i]) {
                        cnt >>= 1;
                        if (k >= cnt) {
                            if ((ans ^ m_masks[i]) > ans) ans ^= m_masks[i];
                            k -= cnt;
                        } else if ((ans ^ m_masks[i]) < ans) {
                            ans ^= m_masks[i];
                        }
                    }
                }
                return ans;
            }
            Tp kth_in_total(Tp k, const Tp &total, Tp base = 0) const {
                Tp cnt = total, ans = base;
                for (size_type i = width() - 1; cnt > 1 && ~i; i--) {
                    if (m_masks[i]) {
                        cnt >>= 1;
                        if (k >= cnt) {
                            if ((ans ^ m_masks[i]) > ans) ans ^= m_masks[i];
                            k -= cnt;
                        } else if ((ans ^ m_masks[i]) < ans)
                            ans ^= m_masks[i];
                    }
                }
                return ans;
            }
            /**
             * @brief 返回向量 `mask` 在所有可表达向量中的排名
             * @return 返回排名，`rank(0) = 0` 表示最小的向量
             */
            Tp rank(Tp mask) const {
                Tp cnt = total_count(), ans = 0;
                for (size_type i = width() - 1; ~i; i--) {
                    if (m_masks[i]) {
                        cnt >>= 1;
                        if (mask >> i & 1) ans += cnt;
                    }
                }
                return ans;
            }
            Tp rank_in_total(Tp mask, const Tp &total) const {
                Tp cnt = total, ans = 0;
                for (size_type i = width() - 1; ~i; i--) {
                    if (m_masks[i]) {
                        cnt >>= 1;
                        if (mask >> i & 1) ans += cnt;
                    }
                }
                return ans;
            }
            /**
             * @brief 枚举所有的基向量
             * @param call 参数为 call(Tp mask)
             */
            template <typename Callback>
            void enumerate_base(Callback &&call) const {
                for (size_type i = width() - 1; ~i; i--) {
                    if (m_masks[i]) call(m_masks[i]);
                }
            }
            /**
             * @brief 枚举所有可表达的向量
             * @param call 参数为 call(Tp mask)
             */
            template <typename Callback>
            void enumerate(Callback &&call) const {
                std::vector<size_type> next(width() + 1, -1);
                for (size_type lst = width(), i = width() - 1; ~i; i--) {
                    if (m_masks[i]) next[lst] = i, lst = i;
                }
                _dfs(width(), 0, next, call);
            }
            /**
             * @brief 查询两位是否绑定
             * @note 即如果存在一个可表达向量 `mask`，使得 `mask >> k1 & 1 != mask >> k2 & 1`，则返回 `false`，否则返回 `true`
             */
            bool is_bonded(size_type k1, size_type k2) const {
                for (size_type i = std::min(k1, k2); i != width(); i++)
                    if ((m_masks[i] >> k1 & 1) != (m_masks[i] >> k2 & 1)) return false;
                return true;
            }
            /**
             * @brief 查询最大异或
             */
            Tp query_max_bitxor(Tp base = 0) const {
                Tp ans = base;
                for (size_type i = width() - 1; ~i; i--)
                    if ((ans ^ m_masks[i]) > ans) ans ^= m_masks[i];
                return ans;
            }
            HamelXorBase<Tp, MAX_WIDTH> &operator+=(const HamelXorBase<Tp, MAX_WIDTH> &rhs) {
                for (size_type i = 0; i != width(); i++)
                    if (rhs.m_masks[i]) insert(rhs.m_masks[i]);
                return *this;
            }
            friend HamelXorBase<Tp, MAX_WIDTH> operator+(const HamelXorBase<Tp, MAX_WIDTH> &a, const HamelXorBase<Tp, MAX_WIDTH> &b) { return HamelXorBase<Tp, MAX_WIDTH>(a) += b; }
        };
    }
    template <HAMEL::size_type MAX_WIDTH, typename = typename std::enable_if<MAX_WIDTH>::type>
    using StaticHamelXorBase32 = HAMEL::HamelXorBase<uint32_t, MAX_WIDTH>;
    template <HAMEL::size_type MAX_WIDTH, typename = typename std::enable_if<MAX_WIDTH>::type>
    using StaticHamelXorBase64 = HAMEL::HamelXorBase<uint64_t, MAX_WIDTH>;
    using DynamicHamelXorBase32 = HAMEL::HamelXorBase<uint32_t, 0>;
    using DynamicHamelXorBase64 = HAMEL::HamelXorBase<uint64_t, 0>;
}
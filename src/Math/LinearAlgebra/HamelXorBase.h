#ifndef __OY_HAMELXORBASE__
#define __OY_HAMELXORBASE__

#include <algorithm>
#include <cstdint>
#include <vector>

#if __has_include(<bit>)
#include <bit>
#else
#include "src/Misc/std_bit.h"
#endif

/**
 * @brief 异或线性基
 * @example OY::StaticHamelXorBase32<MAX_WIDTH> hxb;
 * @example OY::StaticHamelXorBase64<MAX_WIDTH> hxb;
 * @note MAX_WIDTH 最大宽度，插入的数最多有 MAX_WIDTH 位，即 [0, 2^MAX_WIDTH)
 * @note 若干数以任何顺序插入，最终线性基的大小都是一样的，等于主元的数量
 */
namespace OY {
    namespace HAMEL {
        using size_type = uint32_t;
        /**
         * @brief 静态异或线性基
         * @tparam MAX_WIDTH 最大宽度
         */
        template <typename Tp, size_type MAX_WIDTH>
        struct MaskNodes {
            Tp m_val[MAX_WIDTH];
            Tp &operator[](size_type index) { return m_val[index]; }
            Tp operator[](size_type index) const { return m_val[index]; }
            size_type count() const {
                return std::count_if(m_val, m_val + MAX_WIDTH, [&](Tp x) { return x; });
            }
        };
        /**
         * @brief 动态异或线性基
         */
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
            /**
             * @brief 设置线性基维数
             * @param w 线性基维数
             */
            static void set_width(size_type w) {
                static_assert(!MAX_WIDTH, "MAX_WIDTH Must Be 0");
                MaskNodes<Tp, MAX_WIDTH>::s_width = w;
            }
            /**
             * @brief 返回当前线性基维数
             */
            static size_type width() {
                if constexpr (MAX_WIDTH != 0)
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
             * @return 返回新插入的向量所能贡献的最高位，如果插入失败，返回 `-1`
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
             * @note 如果 `mask` 和之前已形成的线性基在异或运算上线性无关，则视为不包含，返回 `false` 。否则视为包含，返回 `true` 。
             * @note 不可用于查询 `0` 是否可表达，`0` 可表达等价于线性基线性相关，等价于 `base_count() < n`
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
             * @param base 表示向量起点，也可以理解为 base 是一个必选的向量
             * @note 允许所有基向量都不选
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
             * @param mask 表示查询的向量，注意这个向量必须是可表达的向量
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
             * @brief 枚举所有可表达的向量，注意这个枚举并不按照大小顺序
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
             * @note 线性基中的向量可以一个也不选
             */
            Tp query_max_bitxor(Tp base = 0) const {
                Tp ans = base;
                for (size_type i = width() - 1; ~i; i--)
                    if ((ans ^ m_masks[i]) > ans) ans ^= m_masks[i];
                return ans;
            }
            /**
             * @brief 查询最小异或
             * @note 线性基中的向量可以一个也不选
             */
            Tp query_min_bitxor(Tp base = 0) const {
                Tp ans = base;
                for (size_type i = width() - 1; ~i; i--)
                    if ((ans ^ m_masks[i]) < ans) ans ^= m_masks[i];
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
    template <HAMEL::size_type MAX_WIDTH, typename = typename std::enable_if<MAX_WIDTH != 0>::type>
    using StaticHamelXorBase32 = HAMEL::HamelXorBase<uint32_t, MAX_WIDTH>;
    template <HAMEL::size_type MAX_WIDTH, typename = typename std::enable_if<MAX_WIDTH != 0>::type>
    using StaticHamelXorBase64 = HAMEL::HamelXorBase<uint64_t, MAX_WIDTH>;
    using DynamicHamelXorBase32 = HAMEL::HamelXorBase<uint32_t, 0>;
    using DynamicHamelXorBase64 = HAMEL::HamelXorBase<uint64_t, 0>;
}

#endif
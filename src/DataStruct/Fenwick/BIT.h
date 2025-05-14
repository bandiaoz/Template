#ifndef __OY_BIT__
#define __OY_BIT__

#include <algorithm>
#include <cstdint>
#include <vector>

#if __has_include(<bit>)
#include <bit>
#else
#include "src/Misc/std_bit.h"
#endif

/**
 * @brief 树状数组
 * @example OY::VectorBIT<Tp> T(length, mapping);
 *          `Tp` 是一个值类型，如果建立空的求和数组，可以不需要 `mapping` 参数。
 *          要求 `Tp` 支持 `+=` 运算符；
 *          区间查询要求支持 `-` 运算符；
 *          单点查询和 `kth` 要求支持 `-=` 运算符； 
 *          `kth` 运算还要求支持 `<=` 运算符。
 */
namespace OY {
    namespace BIT {
        using size_type = uint32_t;
        inline size_type lowbit(size_type x) { return x & -x; }
        template <size_type BUFFER>
        struct StaticBufferWrap {
            template <typename Node>
            struct type {
                using value_type = Node *;
                static Node s_buf[BUFFER];
                static size_type s_use_cnt;
                static void malloc(value_type &x, size_type length) { x = s_buf + s_use_cnt, s_use_cnt += length; }
                static void reset_buffer() { std::fill_n(s_buf, s_use_cnt, 0), s_use_cnt = 0; }
            };
        };
        template <size_type BUFFER>
        template <typename Node>
        Node StaticBufferWrap<BUFFER>::type<Node>::s_buf[BUFFER];
        template <size_type BUFFER>
        template <typename Node>
        size_type StaticBufferWrap<BUFFER>::type<Node>::s_use_cnt;
        template <typename Node>
        struct VectorBuffer {
            using value_type = std::vector<Node>;
            static void malloc(value_type &x, size_type length) { x.assign(length, {}); }
        };
        template <typename Tp, template <typename> typename BufferType = VectorBuffer>
        class Tree {
        public:
            using buffer_type = BufferType<Tp>;
        private:
            size_type m_size, m_length;
            typename buffer_type::value_type m_sum;
            void _add(size_type i, Tp inc) {
                while (i < m_length) m_sum[i] += inc, i += lowbit(i + 1);
            }
        public:
            Tree() = default;
            Tree(size_type length) { resize(length); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            void resize(size_type length) {
                if (length) buffer_type::malloc(m_sum, m_length = std::bit_ceil(m_size = length));
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!length) return;
                buffer_type::malloc(m_sum, m_length = std::bit_ceil(m_size = length));
                for (size_type i = 0; i != length; i++) m_sum[i] = mapping(i);
                for (size_type i = 0; i != m_length; i++) {
                    size_type j = i + lowbit(i + 1);
                    if (j < m_length) m_sum[j] += m_sum[i];
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type size() const { return m_size; }
            /**
             * @brief 单点增值，将下标为 `i` 的元素增加 `inc`
             */
            void add(size_type i, Tp inc) { _add(i, inc); }
            /**
             * @brief 前缀和查询，返回闭区间 `[0, i)` 的和
             */
            Tp presum(size_type i) const {
                i--;
                Tp res{};
                for (size_type j = i; ~j; j -= lowbit(j + 1)) res += m_sum[j];
                return res;
            }
            /**
             * @brief 单点查询，返回下标为 `i` 的元素
             */
            Tp query(size_type i) const {
                Tp res = m_sum[i];
                for (size_type j = i - 1, k = lowbit(i + 1); k >>= 1; j -= lowbit(j + 1)) res -= m_sum[j];
                return res;
            }
            /**
             * @brief 区间查询，返回区间 `[left, right)` 的和
             * @note 允许 `left == right`，此时返回 `0`
             */
            Tp query(size_type left, size_type right) const { return presum(right) - presum(left); }
            /**
             * @brief 全区间查询，返回区间 `[0, n)` 的和
             */
            Tp query_all() const { return m_sum[m_length - 1]; }
            /**
             * @brief 查询第 k 个元素的下标
             * @note 也可以解释为，本方法在求可令 `presum(i+1) > k` 成立的最小的 `i` 。
             *       `k` 必须位于 `[0, query_all())` 之间。
             */
            size_type kth(Tp k) const {
                size_type cursor = -1;
                for (size_type d = m_length >> 1; d; d >>= 1)
                    if (m_sum[cursor + d] <= k) k -= m_sum[cursor += d];
                return cursor + 1;
            }
            /**
             * @brief 对所有位置的值调用回调，即 `call(query(i))`
             */
            template <typename Callback>
            void enumerate(Callback &&call) const {
                for (size_type i = 0; i != m_size; i++) call(query(i));
            }
        };
        template <typename Ostream, typename Tp, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<Tp, BufferType> &x) {
            out << '[';
            x.enumerate([&out, &x, i = 0](Tp val) mutable { out << (i++ ? ", " : "") << val; });
            return out << "]";
        }
    };
    template <typename Tp, BIT::size_type BUFFER = 1 << 22>
    using StaticBIT = BIT::Tree<Tp, BIT::StaticBufferWrap<BUFFER>::template type>;
    template <typename Tp>
    using VectorBIT = BIT::Tree<Tp, BIT::VectorBuffer>;
}

#endif
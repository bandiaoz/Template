#ifndef __OY_MONOSTACK__
#define __OY_MONOSTACK__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

/**
 * @brief 单调栈
 * @example OY::MONOSTACK::Table<Tp, std::less_equal<Tp>, decltype(getter), decltype(pop_call)> stk(
        length, {}, getter, pop_call, std::numeric_limits<Tp>::max()
    ); // 建立一个非严格单调递减栈
 * @note 在比较函数为 `std::less<Tp>` 时，单调栈为严格单调递减栈，可以查询区间最大值；
 *       在比较函数为 `std::less_equal<Tp>` 时，单调栈为非严格单调递减栈，可以查询区间最大值；
 *       在比较函数为 `std::greater<Tp>` 时，单调栈为严格单调递增栈，可以查询区间最小值；
 *       在比较函数为 `std::greater_equal<Tp>` 时，单调栈为非严格单调递增栈，可以查询区间最小值。
 *       也就是满足 `comp(a[i + 1], a[i]) == true`
 */
namespace OY {
    namespace MONOSTACK {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Tp>
        struct PointerGetter {
            Tp *const m_arr;
            Tp operator()(size_type i) const { return m_arr[i]; }
        };
        template <typename Tp>
        struct VectorGetter {
            std::vector<Tp> &m_arr;
            Tp operator()(size_type i) const { return m_arr[i]; }
        };
        template <typename Tp, typename Compare, typename Getter, typename PopCallback = Ignore>
        class Table {
            struct node {
                size_type m_index;
                Tp m_value;
            };
            std::vector<node> m_data;
            size_type m_cur;
            Compare m_comp;
            Getter m_getter;
            PopCallback m_pop_call;
        public:
            /**
             * @brief 构造函数
             * @param length 区间长度
             * @param comp 比较函数
             * @param getter 从区间下标到值的映射函数
             * @param pop_call 每个元素从单调栈里被 `pop` 时调用的回调函数
             * @param max 区间最大值
             * @param forward 区间边界是否为正向延伸，默认为 true
             */
            Table(size_type length, Compare comp, Getter getter, PopCallback pop_call, const Tp &max, bool forward = true) : m_comp(comp), m_getter(getter), m_pop_call(pop_call) {
                m_data.reserve(length + 1);
                m_cur = forward ? -1 : length;
                m_data.push_back({forward ? size_type(-1) : length, max});
            }
            /**
             * @brief 窗口左边界向左拓展一个位置
             * @note 本方法仅在区间边界为反向延伸时使用
             */
            void extend_left() {
                Tp val = m_getter(--m_cur);
                while (!m_comp(val, m_data.back().m_value)) {
                    if constexpr (!std::is_same<PopCallback, Ignore>::value) m_pop_call(m_data.back().m_index, m_cur);
                    m_data.pop_back();
                }
                m_data.push_back({m_cur, val});
            }
            /**
             * @brief 窗口右边界向右拓展一个位置
             * @note 本方法仅在区间边界为正向延伸时使用
             */
            void extend_right() {
                Tp val = m_getter(++m_cur);
                while (!m_comp(val, m_data.back().m_value)) {
                    if constexpr (!std::is_same<PopCallback, Ignore>::value) m_pop_call(m_data.back().m_index, m_cur);
                    m_data.pop_back();
                }
                m_data.push_back({m_cur, val});
            }
            /**
             * @brief 在光标左侧查询区间最值
             * @param left 查询的左边界
             * @return 返回区间最值所在的下标以及其值
             * @note 本方法仅在区间边界为正向延伸时使用，有多个最值时返回最后出现的下标
             */
            const node *raw_query_on_left_side(size_type left) const {
                size_type l = 1, r = m_data.size() - 1;
                while (l != r) {
                    size_type mid = (l + r) / 2;
                    if (m_data[mid].m_index >= left)
                        r = mid;
                    else
                        l = mid + 1;
                }
                return &m_data[l];
            }
            /**
             * @brief 在光标右侧查询区间最值
             * @param right 查询的右边界
             * @return 返回区间最值所在的下标以及其值
             * @note 本方法仅在区间边界为反向延伸时使用，有多个最值时返回最先出现的下标
             */
            const node *raw_query_on_right_side(size_type right) const {
                size_type l = 1, r = m_data.size() - 1;
                while (l != r) {
                    size_type mid = (l + r) / 2;
                    if (m_data[mid].m_index <= right)
                        r = mid;
                    else
                        l = mid + 1;
                }
                return &m_data[l];
            }
            /**
             * @brief 在光标左侧查询区间最值，如果当前光标小于等于 right，则自动向右拓展光标
             * @param left 查询的左边界
             * @param right 查询的右边界
             * @return 返回区间最值所在的下标以及其值
             * @note 本方法仅在区间边界为正向延伸时使用，有多个最值时返回最后出现的下标
             */
            const node *query_on_left_side(size_type left, size_type right) {
                while (m_cur != right) extend_right();
                return raw_query_on_left_side(left);
            }
            /**
             * @brief 在光标右侧查询区间最值，如果当前光标大于等于 left，则自动向左拓展光标
             * @param left 查询的左边界
             * @param right 查询的右边界
             * @return 返回区间最值所在的下标以及其值
             * @note 本方法仅在区间边界为反向延伸时使用，有多个最值时返回最先出现的下标
             */
            const node *query_on_right_side(size_type left, size_type right) {
                while (m_cur != left) extend_left();
                return raw_query_on_right_side(right);
            }
            size_type cursor() const { return m_cur; }
            bool is_finished() const { return (~m_data.front().m_index) ? !m_data.back().m_index : (m_data.back().m_index + 1 == m_data.capacity()); }
        };
    }
    template <typename Tp, typename PopCallback = MONOSTACK::Ignore>
    auto make_MonoMaxStack(MONOSTACK::size_type length, Tp *arr, PopCallback pop_call = PopCallback(), const Tp &max = std::numeric_limits<Tp>::max(), bool forward = true) -> MONOSTACK::Table<Tp, std::less<Tp>, MONOSTACK::PointerGetter<Tp>, PopCallback> { return MONOSTACK::Table<Tp, std::less<Tp>, MONOSTACK::PointerGetter<Tp>, PopCallback>(length, {}, {arr}, pop_call, max, forward); }
    template <typename Tp, typename PopCallback = MONOSTACK::Ignore>
    auto make_MonoMinStack(MONOSTACK::size_type length, Tp *arr, PopCallback pop_call = PopCallback(), const Tp &min = std::numeric_limits<Tp>::min(), bool forward = true) -> MONOSTACK::Table<Tp, std::greater<Tp>, MONOSTACK::PointerGetter<Tp>, PopCallback> { return MONOSTACK::Table<Tp, std::greater<Tp>, MONOSTACK::PointerGetter<Tp>, PopCallback>(length, {}, {arr}, pop_call, min, forward); }
    template <typename Tp, typename PopCallback = MONOSTACK::Ignore>
    auto make_MonoMaxStack(MONOSTACK::size_type length, std::vector<Tp> &arr, PopCallback pop_call = PopCallback(), const Tp &max = std::numeric_limits<Tp>::max(), bool forward = true) -> MONOSTACK::Table<Tp, std::less<Tp>, MONOSTACK::VectorGetter<Tp>, PopCallback> { return MONOSTACK::Table<Tp, std::less<Tp>, MONOSTACK::VectorGetter<Tp>, PopCallback>(length, {}, {arr}, pop_call, max, forward); }
    template <typename Tp, typename PopCallback = MONOSTACK::Ignore>
    auto make_MonoMinStack(MONOSTACK::size_type length, std::vector<Tp> &arr, PopCallback pop_call = PopCallback(), const Tp &min = std::numeric_limits<Tp>::min(), bool forward = true) -> MONOSTACK::Table<Tp, std::greater<Tp>, MONOSTACK::VectorGetter<Tp>, PopCallback> { return MONOSTACK::Table<Tp, std::greater<Tp>, MONOSTACK::VectorGetter<Tp>, PopCallback>(length, {}, {arr}, pop_call, min, forward); }
    template <typename Tp, typename Getter, typename PopCallback = MONOSTACK::Ignore>
    using MonoMaxStack = MONOSTACK::Table<Tp, std::less<Tp>, Getter, PopCallback>;
    template <typename Tp, typename Getter, typename PopCallback = MONOSTACK::Ignore>
    using MonoMinStack = MONOSTACK::Table<Tp, std::greater<Tp>, Getter, PopCallback>;
}

#endif
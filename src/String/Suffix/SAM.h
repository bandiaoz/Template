#ifndef __OY_SAM__
#define __OY_SAM__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

/**
 * @brief 后缀自动机
 * @note 后缀自动机存储了一个字符串的所有子串，并且仅可以表示出所有子串
        SAM 可以看作字符串全体后缀的 ac 自动机的压缩版本，存储了字符串的全体后缀的所有前缀信息，也就存储了字符串的全部子串信息
        SAM 的后缀链接树可以看作字符串全体前缀的后缀路径的压缩版本，存储了字符串的全体前缀的所有后缀信息，也就存储了字符串的全部子串信息
 * @note 一个 SAM 最多有 2n-1 个结点和 3n-4 条转移边。
 * @note 到达某个状态的路径可能不止一条，一个状态对应一些子串的集合
 * @note endpos: 子串在字符串中出现的位置的右端点集合
        引理 1：如果两个子串的 endpos 相同，则其中子串一个必然为另一个的后缀；
        引理 2：两个子串要么 $$\rm{endpos}(s) \in \rm{endpos}(t)$$，要么 $$\rm{endpos}(s) \cap \rm{endpos}(t) = \emptyset$$，
               并且当 $$\rm{endpos}(s) \in \rm{endpos}(t)$$ 时，$$t$$ 是 $$s$$ 的后缀；
        引理 3：将 endpos 相同的子串归为一个等价类，按照长度非递增排序。每个子串都是前一个子串的后缀，且等价类中子串长度是连续的，取遍区间内所有整数值；
                换句话说，对于同一等价类的任意两子串，较短者为较长者的后缀，且等价类中子串长度是连续的，取遍区间内所有整数值。
        以上概括为：同一个状态对应的子串的长度各不相同，而且是连续的若干自然数，其中较短的总是较长的子串的后缀。
 * @note link：状态 v 对应具有相同 endpos 的子串等价类，其中 w 为这些字符串中最长的一个，其他字符串都是 w 的后缀；
               $$\rm{link}(v)$$ 连接到的状态，对应于 w 的后缀中与 w 的 endpos 集合不同且最长的那个，也就是 w 的后缀中在 s 中的出现次数比 w 更多且最长的那个；
        规定初始状态 $$t_0$$ 对应的等价类，只包含空串，且 $$\rm{endpos}(t_0) = \{-1, 0, 1, 2, \cdots, n - 1\}$$；
        引理 4：所有后缀链接构成一棵根节点为 t_0 的树；
        引理 5：以 endpos 集合为节点、集合的包含关系作为边，构造的树（每个子节点的 endpos 集合都包含在父节点的 endpos 集合中），称为 parent tree，与通过后缀链接 link 构造的树相同。
 * @note implement：插入一个长度为 n 的字符串，m_node.size() = n + 1，m_data.size() <= 2n - 1
         prepare 后，m_queue.size() = m_data.size() <= 2n - 1
 */
namespace OY {
    namespace SAM {
        using size_type = uint32_t;
        template <size_type ChildCount>
        struct StaticChildGetter {
            size_type m_child[ChildCount];
            void set_child(size_type index, size_type child) { m_child[index] = child; }
            size_type get_child(size_type index) const { return m_child[index]; }
            void copy_children(const StaticChildGetter<ChildCount> &rhs) { std::copy_n(rhs.m_child, ChildCount, m_child); }
        };
        template <typename ChildGetter>
        struct Automaton {
            struct node : ChildGetter {
                // m_length 表示状态对应的最长子串的长度
                // m_fail，参考 link() 的定义
                size_type m_length, m_fail;
            };
            std::vector<node> m_data;
            std::vector<size_type> m_node, m_queue;
            size_type _newnode() {
                m_data.push_back({});
                return m_data.size() - 1;
            }
            void _init() { m_data.resize(1), m_data[0].m_fail = -1, m_node.push_back(0); }
            Automaton() { _init(); }
            template <typename InitMapping>
            Automaton(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Automaton(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                reserve(length);
                for (size_type i = 0; i != length; i++) push_back(mapping(i));
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            /**
             * @brief 预留空间，可以插入 length 个字符
             */
            void reserve(size_type length) { m_data.clear(), m_data.reserve(length * 2 + 1), m_node.clear(), m_node.reserve(length + 1), m_queue.clear(), _init(); }
            void clear() { m_data.clear(), m_node.clear(), m_queue.clear(), _init(); }
            /**
             * @brief 查询字符串的长度
             */
            size_type size() const { return m_node.size() - 1; }
            bool empty() const { return m_node.size() == 1; }
            template <typename ValueType>
            void push_back(const ValueType &elem) {
                size_type last = m_node.back(), end = _newnode(), child;
                m_node.push_back(end);
                m_data[end].m_length = m_data[last].m_length + 1;
                while (~last) {
                    child = m_data[last].get_child(elem);
                    if (child) break;
                    m_data[last].set_child(elem, end), last = m_data[last].m_fail;
                }
                if (~last) {
                    if (m_data[child].m_length == m_data[last].m_length + 1) {
                        m_data[end].m_fail = child;
                    } else {
                        size_type q = child, newq = _newnode();
                        m_data[newq].m_length = m_data[last].m_length + 1;
                        m_data[newq].m_fail = m_data[q].m_fail;
                        m_data[newq].copy_children(m_data[q]);
                        m_data[q].m_fail = m_data[end].m_fail = newq;
                        do {
                            child = m_data[last].get_child(elem);
                            if (child != q) break;
                            m_data[last].set_child(elem, newq);
                            last = m_data[last].m_fail;
                        } while (~last);
                    }
                }
            }
            /**
             * @brief 预处理，建立 bfs 序 m_queue
             */
            void prepare() {
                std::vector<size_type> cnt(m_node.size());
                for (auto &e : m_data) cnt[e.m_length]++;
                std::partial_sum(cnt.begin(), cnt.end(), cnt.begin());
                m_queue.resize(m_data.size());
                for (size_type i = 0; i != m_data.size(); i++) m_queue[--cnt[m_data[i].m_length]] = i;
            }
            size_type query_node_index(size_type i) const { return m_node[i + 1]; }
            size_type query_fail(size_type node_index) const { return m_data[node_index].m_fail; }
            const node *get_node(size_type node_index) const { return &m_data[node_index]; }
            node *get_node(size_type node_index) { return &m_data[node_index]; }
            const node *get_fail_node(size_type node_index) const { return get_node(query_fail(node_index)); }
            node *get_fail_node(size_type node_index) { return get_node(query_fail(node_index)); }
            template <typename Callback>
            void do_for_extending_nodes(Callback &&call) const {
                for (size_type a : m_queue) call(a);
            }
            template <typename Callback>
            void do_for_failing_nodes(Callback &&call) const {
                for (size_type i = m_queue.size() - 1; ~i; i--) call(m_queue[i]);
            }
        };
    }
    template <SAM::size_type ChildCount = 26>
    using StaticSAM_string = SAM::Automaton<SAM::StaticChildGetter<ChildCount>>;
}

#endif
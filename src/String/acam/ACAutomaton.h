#ifndef __OY_ACAUTOMATON__
#define __OY_ACAUTOMATON__

#include <cstdint>
#include <vector>

/**
 * @brief AC 自动机
 * @example OY::ACAM<ChildCount> ac;
 * @note 模板参数 `size_type ChildCount` ，表示 `AC` 自动机的每个结点里有多少个孩子结点。
 * @note fail 指针指向当前状态「最长可识别后缀」
 */
namespace OY {
    namespace AC {
        using size_type = uint32_t;
        struct Ignore {};
        struct BaseNode {};
        template <typename Node, size_type ChildCount>
        struct Automaton {
            struct node : Node {
                size_type m_child[ChildCount], m_fail;
                bool has_child(size_type index) const { return m_child[index]; }
                void add_child(size_type index, size_type child) {
                    m_child[index] = child;
                }
                size_type get_child(size_type index) const { return m_child[index]; }
            };
            std::vector<node> m_data;
            std::vector<size_type> m_queue;
            size_type _newnode() {
                m_data.push_back({});
                return m_data.size() - 1;
            }
            size_type _get_or_new(size_type x, size_type c) {
                if (!m_data[x].has_child(c)) {
                    size_type y = _newnode();
                    m_data[x].add_child(c, y);
                    return y;
                } else
                    return m_data[x].get_child(c);
            }
            void _init() { m_data.push_back({}); }
            Automaton() { _init(); }
            /**
             * @brief 预留空间
             */
            void reserve(size_type length) { m_data.clear(), m_queue.clear(), m_data.reserve(length + 1), _init(); }
            /**
             * @brief 清空
             */
            void clear() { m_data.clear(), m_data.push_back({}), m_queue.clear(); }
            /**
             * @brief 查询是否为空
             */
            bool empty() const { return m_data.size() == 1; }
            /**
             * @brief 插入模式序列
             * @return 返回序列末尾的结点编号
             */
            template <typename Iterator, typename Mapping>
            size_type insert(Iterator first, Iterator last, Mapping &&map) {
                size_type x = 0;
                for (auto it = first; it != last; ++it) {
                    size_type c = map(*it);
                    x = _get_or_new(x, c);
                }
                return x;
            }
            /**
             * @brief 插入小写字母模式序列，映射到 `[0, 26)` 区间
             * @return 返回序列末尾的结点编号
             */
            size_type insert_lower(const std::string &s) {
                return insert(s.begin(), s.end(), [](char c) { return c - 'a'; });
            }
            /**
             * @brief 构建 AC 自动机
             * @param child_call 回调函数，`child_call(u, v)`，字典树上存在 `u` 到 `v` 的边，不包括虚拟孩子
             * @note 按照 trie 树的 bfs 序，构建 fail 指针，`m_queue` 为所有状态节点的 bfs 序
             * @note `u` 通过字符 `c` 连向 `v`，则 `v` 的 `fail` 指针指向 `u` 的 `fail` 指针的 `c` 孩子。
             */
            template <typename ChildCallback = Ignore>
            void prepare(ChildCallback &&child_call = ChildCallback()) {
                m_queue.reserve(m_data.size() - 1);
                size_type cursor = 0;
                for (size_type i = 0; i != ChildCount; i++) {
                    if (m_data[0].has_child(i)) {
                        size_type child = m_data[0].get_child(i);
                        if constexpr (!std::is_same<typename std::decay<ChildCallback>::type, Ignore>::value) child_call(0, child);
                        m_queue.push_back(child);
                    }
                }
                while (cursor != m_queue.size()) {
                    size_type cur = m_queue[cursor++];
                    for (size_type i = 0; i != ChildCount; i++) {
                        size_type child = m_data[cur].get_child(i);
                        if (child) {
                            if constexpr (!std::is_same<typename std::decay<ChildCallback>::type, Ignore>::value) child_call(cur, child);
                            m_data[child].m_fail = m_data[m_data[cur].m_fail].get_child(i);
                            m_queue.push_back(child);
                        } else { // 如果当前状态没有 i 孩子，则添加一个虚拟孩子，指向 fail 指针的 i 孩子
                            m_data[cur].add_child(i, m_data[m_data[cur].m_fail].get_child(i));
                        }
                    }
                }
            }
            /**
             * @brief 查询失配后的索引
             */
            size_type query_fail(size_type index) const { return m_data[index].m_fail; }
            /**
             * @brief 获取结点指针
             */
            const node *get_node(size_type index) const { return &m_data[index]; }
            node *get_node(size_type index) { return &m_data[index]; }
            /**
             * @brief 获取失配后的结点指针
             */
            const node *get_fail_node(size_type index) const { return get_node(query_fail(index)); }
            /**
             * @brief 获取失配后的结点指针
             */
            node *get_fail_node(size_type index) { return get_node(query_fail(index)); }
            /**
             * @brief 获取 `index` 的 `elem` 孩子结点
             * @note 一定没有空指针，因为 `prepare` 构建了虚拟孩子，连向 fail 指针的 `elem` 孩子
             */
            size_type next(size_type index, size_type elem) const { return m_data[index].get_child(elem); }
            /**
             * @brief 按照扩展顺序对各个结点调用回调函数，即 trie 树的 bfs 序
             * @param call 回调函数，`call(u)`
             * @note 不包括 trie 根节点
             */
            template <typename Callback>
            void do_for_extending_nodes(Callback &&call) const {
                for (auto &a : m_queue) call(a);
            }
            /**
             * @brief 按照失配顺序对各个结点调用回调函数，即 trie 树的 bfs 逆序
             * @param call 回调函数，`call(u)`
             * @note 不包括 trie 根节点
             */
            template <typename Callback>
            void do_for_failing_nodes(Callback &&call) const {
                for (size_type i = m_queue.size() - 1; ~i; i--) call(m_queue[i]);
            }
        };
    }
    template <AC::size_type ChildCount = 26>
    using ACAM = AC::Automaton<AC::BaseNode, ChildCount>;
}

#endif
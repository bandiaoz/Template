#ifndef __OY_TRIE__
#define __OY_TRIE__

#include <cstdint>

/**
 * @brief Trie 前缀树
 * @example OY::StaticTrie<NodeWrap, ChildCount=26> trie;
 * @example
    template <typename Node>
    struct NodeWrap : OY::Trie::BaseNodeWrap<Node> {
        int cnt = 0;
    };
    using Trie = OY::StaticTrie<NodeWrap, ChildCount>;
    using Node = Trie::node;
 */
namespace OY {
    namespace Trie {
        using size_type = uint32_t;
        struct Ignore {};
        /**
         * @brief NodeWrap 的基类，如果需要使用，需要在 insert 后修改
         */
        template <typename Node>
        struct BaseNodeWrap {
            bool m_is_end;
        };
        /**
         * @brief 静态字符集大小为 ChildCount 的子节点获取器
         * @tparam NodeWrap 节点包装器
         * @tparam ChildCount 字符集大小
         */
        template <template <typename> typename NodeWrap, size_type ChildCount>
        struct StaticChildGetter : NodeWrap<StaticChildGetter<NodeWrap, ChildCount>> {
            size_type m_child[ChildCount];
            void set_child(size_type index, size_type child) { m_child[index] = child; }
            /**
             * @brief 获取子节点编号，如果子节点不存在，返回 0
             */
            size_type get_child(size_type index) const { return m_child[index]; }
        };
        template <typename Node>
        struct Tree {
            using node = Node;
            std::vector<node> m_data;
            template <typename Mapping, typename Modify = Ignore>
            node *_insert(size_type cur, size_type index, size_type length, Mapping &&mapping, Modify &&modify = Modify()) {
                if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(&m_data[cur]);
                if (index != length) {
                    size_type c = mapping(index), child = m_data[cur].get_child(c);
                    if (!child) {
                        m_data.push_back({});
                        m_data[cur].set_child(c, child = m_data.size() - 1);
                    }
                    return _insert(child, index + 1, length, mapping, modify);
                } else
                    return &m_data[cur];
            }
            template <typename Mapping>
            node *_find(size_type cur, size_type index, size_type length, Mapping &&mapping) const {
                size_type c = mapping(index), child = m_data[cur].get_child(c);
                if (!child) return nullptr;
                if (++index == length) return (node *)&m_data[child];
                return _find(child, index, length, mapping);
            }
            template <typename Mapping, typename Modify>
            void _trace(size_type cur, size_type index, size_type length, Mapping &&mapping, Modify &&modify) {
                if (index != length) {
                    size_type c = mapping(index), child = m_data[cur].get_child(c);
                    if (child) _trace(child, index + 1, length, mapping, modify);
                }
                modify(&m_data[cur]);
            }
            /**
             * @brief 初始化 trie 树
             * @note 根节点为 `m_data[0]`
             */
            void _init() { m_data.push_back({}); }
            Tree() { _init(); }
            void reserve(size_type length) { m_data.clear(), m_data.reserve(length + 1), _init(); }
            void clear() { m_data.clear(), _init(); }
            /**
             * @brief 根据 `index` 获取节点指针
             */
            node *get_node(size_type index) const { return (node *)&m_data[index]; }
            /**
             * @brief 插入字符串
             * @param length 要插入的字符串长度
             * @param mapping 要插入的字符串从下标到元素的映射
             * @param modify 对插入路径上的节点（包括根节点）依次调用的回调函数，`modify(node*)`
             * @return 返回插入字符串的节点
             */
            template <typename Mapping, typename Modify = Ignore>
            node *insert(size_type length, Mapping &&mapping, Modify &&modify = Modify()) { return _insert(0, 0, length, mapping, modify); }
            /**
             * @brief 插入小写字母字符串
             * @param s 小写字母字符串
             * @param modify 对插入路径上的节点（包括根节点）依次调用的回调函数，`modify(node*)`
             * @return 返回插入字符串的节点
             */
            template <typename Modify = Ignore>
            node *insert_lower(const std::string &s, Modify &&modify = Modify()) {
                auto lower_map = [&](size_type i) { return s[i] - 'a'; };
                return insert(s.size(), lower_map, modify);
            }
            /**
             * @brief 搜索字符串，如果找到返回节点指针，否则返回 nullptr
             * @param length 要搜索的字符串长度
             * @param mapping 要搜索的字符串从下标到元素的映射
             */
            template <typename Mapping>
            node *find(size_type length, Mapping &&mapping) const { return _find(0, 0, length, mapping); }
            /**
             * @brief 搜索小写字母字符串，如果找到返回节点指针，否则返回 nullptr
             */
            node *find_lower(const std::string &s) const {
                auto lower_map = [&](size_type i) { return s[i] - 'a'; };
                return find(s.size(), lower_map);
            }
            /**
             * @brief 从根开始回溯字符串
             * @param length 要回溯的字符串长度
             * @param mapping 要回溯的字符串从下标到元素的映射
             * @param modify 对回溯路径上的节点依次调用的回调函数，`modify(node*)`
             */
            template <typename Mapping, typename Modify>
            void trace(size_type length, Mapping &&mapping, Modify &&modify) { return _trace(0, 0, length, mapping, modify); }
            /**
             * @brief 从根开始回溯小写字母字符串
             * @param s 小写字母字符串
             * @param modify 对回溯路径上的节点依次调用的回调函数，`modify(node*)`
             */
            template <typename Modify>
            void trace_lower(const std::string &s, Modify &&modify) {
                auto lower_map = [&](size_type i) { return s[i] - 'a'; };
                return _trace(0, 0, s.size(), lower_map, modify);
            }
        };
    }
    template <template <typename> typename NodeWrap = Trie::BaseNodeWrap, Trie::size_type ChildCount = 26>
    using StaticTrie = Trie::Tree<Trie::StaticChildGetter<NodeWrap, ChildCount>>;
}

#endif
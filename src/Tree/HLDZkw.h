#ifndef __OY_HLDZKW__
#define __OY_HLDZKW__

#include <cstdint>

#include "src/DataStruct/Segtree/ZkwTree.h"
#include "src/Tree/HeavyLightDecomposition.h"

/**
 * @brief 树上线段树
 * @example auto zkw = OY::make_lazy_HLDZkw<Info, Tag, InitClearLazy>(&tree, mapping, op, map, com);
 * @example auto zkw = OY::make_HLDZkw<Info>(&tree, op, mapping);
 * @note 其中 mapping 的参数为树上节点编号
 */
namespace OY {
    namespace HLDZKW {
        using size_type = uint32_t;
        template <typename Tree, typename Node>
        struct Table {
            using table_type = ZKW::Tree<Node>;
            using value_type = typename table_type::value_type;
            using modify_type = typename table_type::modify_type;
            using node = typename table_type::node;
            Tree *m_rooted_tree;
            HLD::Table<Tree> m_hld;
            table_type m_zkw;
            template <typename InitMapping = ZKW::Ignore>
            Table(Tree *rooted_tree = nullptr, InitMapping mapping = InitMapping()) { reset(rooted_tree, mapping); }
            /**
             * @brief 重置线段树
             * @param rooted_tree 树
             * @param mapping 映射函数，参数为树上节点编号
             */
            template <typename InitMapping = ZKW::Ignore>
            void reset(Tree *rooted_tree, InitMapping mapping = InitMapping()) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_hld.reset(rooted_tree);
                if constexpr (!std::is_same<InitMapping, ZKW::Ignore>::value)
                    m_zkw.resize(m_rooted_tree->vertex_cnt(), [&](size_type i) { return mapping(m_hld.m_seq[i]); });
                else
                    m_zkw.resize(m_rooted_tree->vertex_cnt());
            }
            /**
             * @brief 对节点编号为 `u` 的节点进行赋值
             */
            void modify(size_type u, const value_type &val) {
                m_hld.do_for_vertex(u, [&](size_type pos) { m_zkw.modify(pos, val); });
            }
            /**
             * @brief 对节点编号为 `u` 的节点进行修改
             */
            void add(size_type u, const modify_type &modify) {
                m_hld.do_for_vertex(u, [&](size_type pos) { m_zkw.add(pos, modify); });
            }
            /**
             * @brief 对节点 `u` 和 `v` 之间的路径进行修改
             */
            template <bool LCA>
            void add_path(size_type u, size_type v, const modify_type &modify) {
                m_hld.template do_for_path<LCA>(u, v, [&](size_type l, size_type r) { m_zkw.add(l, r, modify); });
            }
            /**
             * @brief 对节点 `u` 的子树进行修改
             */
            void add_subtree(size_type u, const modify_type &modify) {
                m_hld.do_for_subtree(u, [&](size_type l, size_type r) { m_zkw.add(l, r, modify); });
            }
            /**
             * @brief 查询节点编号为 `u` 的节点的值
             */
            value_type query(size_type u) const {
                return m_hld.do_for_vertex(u, [&](size_type pos) { return m_zkw.query(pos); });
            }
            /**
             * @brief 查询节点 `u` 和 `v` 之间的路径的值
             */
            template <bool LCA>
            value_type query_path(size_type u, size_type v) const {
                value_type res;
                bool first = true;
                m_hld.template do_for_path<LCA>(u, v, [&](size_type l, size_type r) {
                    if (first)
                        res = m_zkw.query(l, r), first = false;
                    else
                        res = node::op(res, m_zkw.query(l, r));
                });
                return res;
            }
            /**
             * @brief 查询节点 `root` 的子树的值
             */
            value_type query_subtree(size_type root) const {
                return m_hld.do_for_subtree(root, [&](size_type l, size_type r) { return m_zkw.query(l, r); });
            }
            /**
             * @brief 查询整棵树的值
             */
            value_type query_all() const { return m_zkw.query_all(); }
        };
        template <typename Ostream, typename Tree, typename Node>
        Ostream &operator<<(Ostream &out, const Table<Tree, Node> &x) { // http://mshang.ca/syntree/
            x.m_rooted_tree->tree_dp_vertex(
                ~x.m_rooted_tree->m_root ? x.m_rooted_tree->m_root : 0, [&](size_type a, size_type) { out << '[' << x.query(a); }, {}, [&](size_type) { out << ']'; });
            return out;
        }
    }
    template <typename Tp, typename Tree, typename Operation, typename InitMapping = ZKW::Ignore, typename TableType = HLDZKW::Table<Tree, ZKW::CustomNode<Tp, Operation>>>
    auto make_HLDZkw(Tree *rooted_tree, Operation op, InitMapping mapping = InitMapping()) -> TableType { return TableType(rooted_tree, mapping); }
    template <typename Tp, typename Tree, typename InitMapping = ZKW::Ignore, typename TableType = HLDZKW::Table<Tree, ZKW::CustomNode<Tp, const Tp &(*)(const Tp &, const Tp &)>>>
    auto make_HLDZkw(Tree *rooted_tree, const Tp &(*op)(const Tp &, const Tp &), InitMapping mapping = InitMapping()) -> TableType { return TableType::node::s_op = op, TableType(rooted_tree, mapping); }
    template <typename Tp, typename Tree, typename InitMapping = ZKW::Ignore, typename TableType = HLDZKW::Table<Tree, ZKW::CustomNode<Tp, Tp (*)(Tp, Tp)>>>
    auto make_HLDZkw(Tree *rooted_tree, Tp (*op)(Tp, Tp), InitMapping mapping = InitMapping()) -> TableType { return TableType::node::s_op = op, TableType(rooted_tree, mapping); }
    template <typename ValueType, typename ModifyType, bool InitClearLazy, typename Tree, typename InitMapping, typename Operation, typename Mapping, typename Composition, typename TableType = HLDZKW::Table<Tree, ZKW::CustomLazyNode<ValueType, ModifyType, Operation, Mapping, Composition, InitClearLazy>>>
    auto make_lazy_HLDZkw(Tree *rooted_tree, InitMapping mapping, Operation op, Mapping map, Composition com, const ModifyType &default_modify = ModifyType()) -> TableType { return TableType::node::s_default_modify = default_modify, TableType(rooted_tree, mapping); }
    template <typename Tree, typename Tp>
    using HLDZkwLazySumTree = HLDZKW::Table<Tree, ZKW::LazyNode<Tp, Tp>>;
}

#endif
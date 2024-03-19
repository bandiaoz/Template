/**
 * @title Directed_MST
 * @brief 有向图最小树形图
 * @return pair(cost, fa) if no solution, return {0, {}}
 * @link https://judge.yosupo.jp/problem/directedmst
 * @link https://loj.ac/p/140
 */

#pragma once

#include <cassert>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

#include "../DataStruct/DSU.hpp"
#include "../DataStruct/RollbackDSU.hpp"

template <class W>
struct MinCostArborescenceGraph {
   private:
    struct SkewHeapNode {
        int l, r, from, to;
        W weight, lz;
    };
    std::vector<SkewHeapNode> nodes;
    std::vector<int> heap;

    void apply(int i, W upd) {
        nodes[i].weight -= upd;
        nodes[i].lz += upd;
    }
    void push(int i) {
        if (nodes[i].l != -1) apply(nodes[i].l, nodes[i].lz);
        if (nodes[i].r != -1) apply(nodes[i].r, nodes[i].lz);
        nodes[i].lz = W(0);
    }
    int merge(int u, int v) {
        if (u == -1 || v == -1) return (u == -1 ? v : u);
        if (nodes[v].weight < nodes[u].weight) std::swap(u, v);
        push(u);
        nodes[u].r = merge(nodes[u].r, v);
        std::swap(nodes[u].l, nodes[u].r);
        return u;
    }
    void pop(int v) {
        push(heap[v]);
        heap[v] = merge(nodes[heap[v]].l, nodes[heap[v]].r);
    }

   public:
    explicit MinCostArborescenceGraph(int n, int m = 0) : heap(n, -1) {
        nodes.reserve(m);
    }

    void addEdge(int from, int to, W weight) {
        assert(0 <= from && from < heap.size() && 0 <= to && to < heap.size());
        nodes.push_back(SkewHeapNode{-1, -1, from, to, weight, W(0)});
        heap[to] = merge(heap[to], nodes.size() - 1);
    }

    template <class WSum = W>
    std::pair<WSum, std::vector<int>> solve(int root) {
        assert(0 <= root && root < heap.size());
        auto ans = WSum(0);
        std::vector edge(heap.size(), -1);
        std::vector<std::pair<int, int>> cycles;
        DSU dsu(heap.size());
        RollbackDSU dsu_back(heap.size());
        for (int i = 0; i < heap.size(); i++) {
            if (i == root) continue;
            int v = i;
            while (true) {
                if (heap[v] == -1) return {W(0), {}};
                edge[v] = heap[v];
                ans += nodes[edge[v]].weight;
                apply(edge[v], nodes[edge[v]].weight);
                if (dsu.merge(v, dsu_back.leader(nodes[edge[v]].from))) {
                    break;
                }

                int vnext = dsu_back.leader(nodes[edge[v]].from),
                    t = dsu_back.time();
                while (dsu_back.merge(v, vnext)) {
                    heap[dsu_back.leader(v)] = merge(heap[v], heap[vnext]);
                    v = dsu_back.leader(v);
                    vnext = dsu_back.leader(nodes[edge[vnext]].from);
                }
                cycles.emplace_back(edge[v], t);

                while (heap[v] != -1 && dsu_back.same(nodes[heap[v]].from, v)) {
                    pop(v);
                }
            }
        }

        for (auto it = rbegin(cycles); it != rend(cycles); ++it) {
            int vrepr = dsu_back.leader(nodes[it->first].to);
            dsu_back.rollback(it->second);
            int vinc = dsu_back.leader(nodes[edge[vrepr]].to);
            edge[vinc] = std::exchange(edge[vrepr], it->first);
        }

        for (int i = 0; i < heap.size(); i++) {
            edge[i] = (i == root ? -1 : nodes[edge[i]].from);
        }
        return {ans, edge};
    }
};

using ll = long long;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, root;
    std::cin >> n >> m >> root;
    MinCostArborescenceGraph<int> g(n, m);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        std::cin >> u >> v >> w;
        g.addEdge(u, v, w);
    }

    auto [cost, fa] = g.solve<ll>(root);
    std::cout << cost << '\n';
    for (int i = 0; i < n; i++) {
        std::cout << (i == root ? root : fa[i]) << " \n"[i == n - 1];
    }

    return 0;
}
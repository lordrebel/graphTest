#pragma once
#include "graph.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <algorithm>

namespace graphTest {
    enum SCCAlgo {
        Tarjan,
        Kosaraju
    };
}

namespace {
    template<typename DataT>
    void tarjanDFS(typename graphTest::Graph<DataT>::Pointer u,
                   const graphTest::Graph<DataT>* graph,
                   int& timer,
                   std::unordered_map<typename graphTest::Graph<DataT>::Pointer, int>& dfn,
                   std::unordered_map<typename graphTest::Graph<DataT>::Pointer, int>& low,
                   std::stack<typename graphTest::Graph<DataT>::Pointer>& st,
                   std::unordered_set<typename graphTest::Graph<DataT>::Pointer>& in_st,
                   std::vector<std::vector<typename graphTest::Graph<DataT>::Pointer>>& sccs) {
        typedef typename graphTest::Graph<DataT>::Pointer Pointer;
        dfn[u] = low[u] = ++timer;
        st.push(u);
        in_st.insert(u);

        std::vector<typename graphTest::Graph<DataT>::Edge> outEdges;
        graph->getOutEdges(u, outEdges);
        for (const auto& edge : outEdges) {
            Pointer v = edge.to.get_data();
            if (dfn.find(v) == dfn.end()) {
                tarjanDFS<DataT>(v, graph, timer, dfn, low, st, in_st, sccs);
                low[u] = std::min(low[u], low[v]);
            } else if (in_st.find(v) != in_st.end()) {
                low[u] = std::min(low[u], dfn[v]);
            }
        }

        if (dfn[u] == low[u]) {
            std::vector<Pointer> component;
            while (true) {
                Pointer v = st.top();
                st.pop();
                in_st.erase(v);
                component.push_back(v);
                if (v == u) break;
            }
            sccs.push_back(component);
        }
    }

    template<typename DataT>
    std::vector<std::vector<typename graphTest::Graph<DataT>::Pointer>> getSCCTarjan(const graphTest::Graph<DataT> * graph) {
        typedef typename graphTest::Graph<DataT>::Pointer Pointer;
        std::vector<std::vector<Pointer>> sccs;
        std::unordered_map<Pointer, int> dfn, low;
        std::stack<Pointer> st;
        std::unordered_set<Pointer> in_st;
        int timer = 0;

        std::vector<Pointer> vertices = graph->getAllVertexs();
        for (Pointer v : vertices) {
            if (dfn.find(v) == dfn.end()) {
                tarjanDFS<DataT>(v, graph, timer, dfn, low, st, in_st, sccs);
            }
        }
        return sccs;
    }

    template<typename DataT>
    void kosarajuDFS1(typename graphTest::Graph<DataT>::Pointer u,
                      const graphTest::Graph<DataT>* graph,
                      std::unordered_set<typename graphTest::Graph<DataT>::Pointer>& visited,
                      std::vector<typename graphTest::Graph<DataT>::Pointer>& order) {
        typedef typename graphTest::Graph<DataT>::Pointer Pointer;
        visited.insert(u);
        std::vector<typename graphTest::Graph<DataT>::Edge> outEdges;
        graph->getOutEdges(u, outEdges);
        for (const auto& edge : outEdges) {
            Pointer v = edge.to.get_data();
            if (visited.find(v) == visited.end()) {
                kosarajuDFS1<DataT>(v, graph, visited, order);
            }
        }
        order.push_back(u);
    }

    template<typename DataT>
    void kosarajuDFS2(typename graphTest::Graph<DataT>::Pointer u,
                      const graphTest::Graph<DataT>* graph,
                      std::unordered_set<typename graphTest::Graph<DataT>::Pointer>& visited,
                      std::vector<typename graphTest::Graph<DataT>::Pointer>& component) {
        typedef typename graphTest::Graph<DataT>::Pointer Pointer;
        visited.insert(u);
        component.push_back(u);
        std::vector<typename graphTest::Graph<DataT>::Edge> inEdges;
        graph->getInEdges(u, inEdges);
        for (const auto& edge : inEdges) {
            Pointer v = edge.from.get_data();
            if (visited.find(v) == visited.end()) {
                kosarajuDFS2<DataT>(v, graph, visited, component);
            }
        }
    }

    template<typename DataT>
    std::vector<std::vector<typename graphTest::Graph<DataT>::Pointer>> getSCCKosaraju(const graphTest::Graph<DataT> * graph) {
        typedef typename graphTest::Graph<DataT>::Pointer Pointer;
        std::vector<Pointer> order;
        std::unordered_set<Pointer> visited;
        std::vector<Pointer> vertices = graph->getAllVertexs();

        for (Pointer v : vertices) {
            if (visited.find(v) == visited.end()) {
                kosarajuDFS1<DataT>(v, graph, visited, order);
            }
        }

        visited.clear();
        std::vector<std::vector<Pointer>> sccs;
        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            Pointer v = *it;
            if (visited.find(v) == visited.end()) {
                std::vector<Pointer> component;
                kosarajuDFS2<DataT>(v, graph, visited, component);
                sccs.push_back(component);
            }
        }
        return sccs;
    }
}

namespace graphTest {
    template<typename DataT>
    std::vector<std::vector<typename Graph<DataT>::Pointer>> getSCC(const Graph<DataT> * graph, SCCAlgo algo) {
        if (!graph) {
            std::cerr << "Graph is null pointer" << std::endl;
            return {};
        } else if (!graph->isDirected()) {
            std::cerr << "Graph is not directed" << std::endl;
            return {};
        } else {
            switch(algo) {
                case Tarjan:
                    return getSCCTarjan(graph);
                case Kosaraju:
                    return getSCCKosaraju(graph);
                default:
                    std::cerr << "Unknown algorithm" << std::endl;
                    return {};
            }
        }
    }
}

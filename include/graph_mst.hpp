
#include "basicAlgo.hpp"
#include "graph.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace graphTest {
namespace {
template <typename T>
std::vector<std::unique_ptr<Graph<T>>> getMSTsKruskal(const Graph<T> *graph) {
  auto edges = graph->getAllEdges();
  std::sort(
      edges.begin(), edges.end(),
      [](const typename Graph<T>::Edge &a, const typename Graph<T>::Edge &b) {
        return a.weight_ < b.weight_;
      });
  UnionFind<T> uf;
  std::unordered_map< typename Graph<T>::Pointer, std::vector<std::pair<typename Graph<T>::Pointer, int>>> edge_map;
  for(auto & e:edges){
    uf.add(e.from.get_data());
    uf.add(e.to.get_data());
    if(uf.findParent(e.from.get_data()) != uf.findParent(e.to.get_data())) {
        uf.merge(e.from.get_data(), e.to.get_data());
        edge_map[e.from.get_data()].push_back({e.to.get_data(), e.weight_});
        edge_map[e.to.get_data()].push_back({e.from.get_data(), e.weight_});
    }
  }
    auto groups = uf.getGroups();
    std::vector<std::unique_ptr<Graph<T>>> mst_graphs;
    for (const auto &pair : groups) {
        const auto &group_nodes = pair.second;
        std::unordered_set<T*> node_set(group_nodes.begin(), group_nodes.end());
        auto mst_graph = std::make_unique<UnDirectedGraph<T>>();
        for (const auto &node : group_nodes) {
            for(auto p:edge_map[node]){
                if(node_set.count(p.first)){
                    mst_graph->addEdge(node, p.first, p.second);
                }
            }
        }
        mst_graphs.push_back(std::move(mst_graph));
    }
    return mst_graphs;
}

template <typename T>
std::vector<std::unique_ptr<Graph<T>>> getMSTsPrim(const Graph<T> *graph) {
  std::cerr << "not finished";
  exit(1);
}

} // namespace
enum MSTAlgo { KRUSKAL, PRIM };
template <typename T>
std::vector<std::unique_ptr<Graph<T>>> getMSTs(const Graph<T> *graph,
                                               MSTAlgo algo = KRUSKAL) {
  if (graph == nullptr) {
    std::cerr << "input graph is nullptr" << std::endl;
    return {};
  }
  if (graph->isDirected()) {
    std::cerr << "MST only applies to undirected graph" << std::endl;
    return {};
  }
  switch (algo) {
  case KRUSKAL:
    return getMSTsKruskal<T>(graph);
  case PRIM:
    return getMSTsPrim<T>(graph);
  }
  return {};
}

} // namespace graphTest
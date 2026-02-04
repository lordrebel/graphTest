
#include "basicAlgo.hpp"
#include "graph.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

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
  std::unordered_set<T*> unvisited;
  for (const auto &v : graph->getAllVertexs()) {
    unvisited.insert(v);
  }
  std::vector<std::unique_ptr<Graph<T>>> mst_graphs;
  while (!unvisited.empty()) {
    auto mst_graph = std::make_unique<UnDirectedGraph<T>>();
    T *start_node = *unvisited.begin();
    mst_graph->addVertex(start_node);
    unvisited.erase(start_node);
    using EdgeType = typename Graph<T>::Edge;
    auto cmp = [](const EdgeType &a, const EdgeType &b) { return a.weight_ > b.weight_; };
    std::priority_queue<EdgeType, std::vector<EdgeType>, decltype(cmp)> edge_pq(cmp);
    std::vector<EdgeType> edges;
    graph->getOutEdges(start_node, edges);
    for (const auto &e : edges) {
      edge_pq.push(e);
    }
    while (!edge_pq.empty()) {
      auto cur_edge = edge_pq.top();
      edge_pq.pop();
      T *to_node = cur_edge.to.get_data();
      if (unvisited.count(to_node) == 0) {
        continue;
      }
      // add edge to mst
      mst_graph->addEdge(cur_edge.from.get_data(), to_node, cur_edge.weight_);
      unvisited.erase(to_node);
      // add new edges
      graph->getOutEdges(to_node, edges);
      for (const auto &e : edges) {
        if (unvisited.count(e.to.get_data()) > 0) {
          edge_pq.push(e);
        }
      }
    }
    mst_graphs.push_back(std::move(mst_graph));
  }
  return mst_graphs;
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
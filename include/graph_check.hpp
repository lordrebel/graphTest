#pragma once
#include "graph.hpp"
#include <iostream>
namespace graphTest {

namespace {
template <typename dataT> bool hasCircleDirected(Graph<dataT> *g) {
  // DFS 三色标记法 for 有向图
  // TODO
  if (!g->isDirected()) {
    std::cerr << "function for directed graph\n";
    return false;
  }
  std::unordered_map<typename Graph<dataT>::Pointer, int> color;
  auto vertexs = g->getAllVertexs();
  for (auto v : vertexs) {
    color[v] = 0;
  }

  std::function<bool(typename Graph<dataT>::Pointer)> dfs =
      [&](typename Graph<dataT>::Pointer node) -> bool {
    color[node] = 1;

    for (auto neighbor : g->getNext(node)) {
      if (color[neighbor] == 1) {
        return true;
      }
      if (color[neighbor] == 0) {
        if (dfs(neighbor)) {
          return true;
        }
      }
    }

    color[node] = 2;
    return false;
  };

  for (auto v : vertexs) {
    if (color[v] == 0) {
      if (dfs(v)) {
        return true;
      }
    }
  }
  return false;
}

template <typename dataT> bool hasCircleUNDirected(Graph<dataT> *g) {
  if (g->isDirected()) {
    std::cerr << "function for undirected graph\n";
    return false;
  }
  std::unordered_set<typename Graph<dataT>::Pointer> visited;

  std::function<bool(typename Graph<dataT>::Pointer,
                     typename Graph<dataT>::Pointer)>
      dfs = [&](typename Graph<dataT>::Pointer node,
                typename Graph<dataT>::Pointer parent) -> bool {
    visited.insert(node);

    for (auto neighbor : g->getNext(node)) {
      if (neighbor == parent) {
        continue;
      }
      if (visited.count(neighbor)) {
        return true;
      }
      if (dfs(neighbor, node)) {
        return true;
      }
    }
    return false;
  };

  auto vertexs = g->getAllVertexs();
  for (auto v : vertexs) {
    if (!visited.count(v)) {
      if (dfs(v, nullptr)) {
        return true;
      }
    }
  }
  return false;
}
}

template <typename dataT> bool hasCircle(Graph<dataT> *g) {
  // DFS 三色标记法 for 有向图
  if (g->isDirected()) {
    return hasCircleDirected(g);
  } else {
    return hasCircleUNDirected(g);
  }
}

template <typename dataT> bool isSame(Graph<dataT> *g1, Graph<dataT> *g2) {
  auto g1_v=g1->getAllVertexs();
  auto g2_v=g2->getAllVertexs();
  sort(g1_v.begin(),g1_v.end());
  sort(g2_v.begin(),g2_v.end());
  if(g1_v!=g2_v) return false;
  for(auto item:g1_v){
    std::vector<typename Graph<dataT>::Edge> g1_edges,g2_edges;
    g1->getEdges(item, g1_edges);
    g2->getEdges(item, g2_edges);
    sort(g1_edges.begin(),g1_edges.end(), [](const Graph<dataT>::Edge& e1, const Graph<dataT>::Edge& e2){
      return e1.to.get_data()<e2.to.get_data();
    });
    sort(g2_edges.begin(),g2_edges.end(),[](const Graph<dataT>::Edge& e1, const Graph<dataT>::Edge& e2){
      return e1.to.get_data()<e2.to.get_data();
    });
    if(g1_edges!=g2_edges) return false;
  }
  return true;
}
}// namespace graphTest
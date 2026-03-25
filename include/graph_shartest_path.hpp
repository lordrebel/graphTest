#pragma once

#include "graph.hpp"
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace {
using graphTest::Graph;

template <typename T>
std::vector<typename Graph<T>::Edge>
ShortestPathOneSourceDIJKSTRA(Graph<T> *graph, T *source) {
  using Pointer = typename Graph<T>::Pointer;
  using Edge = typename Graph<T>::Edge;
  if (!graph || !source)
    return {};

  auto all_edges = graph->getAllEdges();
  for (const auto &edge : all_edges) {
    if (edge.weight_ < 0) {
      std::cerr << "DIJKSTRA cannot handle negative weight edges" << std::endl;
      return {};
    }
  }

  std::unordered_map<Pointer, int> dist;
  std::unordered_map<Pointer, Pointer> parent;
  std::unordered_map<Pointer, Edge> parent_edge;

  auto vertices = graph->getAllVertexs();
  for (auto v : vertices) {
    dist[v] = INT_MAX;
  }
  dist[source] = 0;

  using P = std::pair<int, Pointer>;
  std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
  pq.push({0, source});

  while (!pq.empty()) {
    auto top = pq.top();
    pq.pop();
    int d = top.first;
    Pointer u = top.second;

    if (d > dist[u])
      continue;

    std::vector<Edge> edges;
    graph->getOutEdges(u, edges);

    for (const auto &edge : edges) {
      Pointer v = edge.to.get_data();
      int weight = edge.weight_;
      if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
        dist[v] = dist[u] + weight;
        parent[v] = u;
        parent_edge.insert_or_assign(v, edge);
        pq.push({dist[v], v});
      }
    }
  }

  std::vector<Edge> res;
  for (auto v : vertices) {
    if (v != source && parent.find(v) != parent.end()) {
      res.push_back(parent_edge.at(v));
    }
  }
  return res;
}

template <typename T>
std::vector<typename Graph<T>::Edge>
ShortestPathOneSourceBELLMAN_FORD(Graph<T> *graph, T *source) {
  using Pointer = typename Graph<T>::Pointer;
  using Edge = typename Graph<T>::Edge;
  if (!graph || !source)
    return {};

  std::unordered_map<Pointer, int> dist;
  std::unordered_map<Pointer, Pointer> parent;
  std::unordered_map<Pointer, Edge> parent_edge;

  auto vertices = graph->getAllVertexs();
  for (auto v : vertices) {
    dist[v] = INT_MAX;
  }
  dist[source] = 0;

  size_t V = vertices.size();
  auto edges = graph->getAllEdges();

  for (size_t i = 1; i < V; ++i) {
    for (const auto &edge : edges) {
      Pointer u = edge.from.get_data();
      Pointer v = edge.to.get_data();
      int weight = edge.weight_;
      if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
        dist[v] = dist[u] + weight;
        parent[v] = u;
        parent_edge.insert_or_assign(v, edge);
      }
    }
  }

  for (const auto &edge : edges) {
    Pointer u = edge.from.get_data();
    Pointer v = edge.to.get_data();
    int weight = edge.weight_;
    if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
      std::cerr << "Graph contains a negative-weight cycle" << std::endl;
      return {};
    }
  }

  std::vector<Edge> res;
  for (auto v : vertices) {
    if (v != source && parent.find(v) != parent.end()) {
      res.push_back(parent_edge.at(v));
    }
  }
  return res;
}

template <typename T>
std::unordered_map<typename Graph<T>::Pointer,
                   std::vector<typename Graph<T>::Edge>>
ShortestPathAllPairsFLOYD_WARSHALL(Graph<T> *graph) {
  using Pointer = typename Graph<T>::Pointer;
  using Edge = typename Graph<T>::Edge;
  if (!graph)
    return {};

  auto vertices = graph->getAllVertexs();
  std::unordered_map<Pointer, std::unordered_map<Pointer, int>> dist;
  std::unordered_map<Pointer, std::unordered_map<Pointer, Pointer>> next;
  std::unordered_map<Pointer, std::unordered_map<Pointer, Edge>> edge_map;

  for (auto u : vertices) {
    for (auto v : vertices) {
      if (u == v)
        dist[u][v] = 0;
      else
        dist[u][v] = INT_MAX;
    }
  }

  auto edges = graph->getAllEdges();
  for (const auto &edge : edges) {
    Pointer u = edge.from.get_data();
    Pointer v = edge.to.get_data();
    dist[u][v] = edge.weight_;
    next[u][v] = v;
    edge_map[u].insert_or_assign(v, edge);
  }

  for (auto k : vertices) {
    for (auto i : vertices) {
      for (auto j : vertices) {
        if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
          if (dist[i][k] + dist[k][j] < dist[i][j]) {
            dist[i][j] = dist[i][k] + dist[k][j];
            next[i][j] = next[i][k];
          }
        }
      }
    }
  }

  for (auto v : vertices) {
    if (dist[v][v] < 0) {
      std::cerr << "Graph contains a negative-weight cycle" << std::endl;
      return {};
    }
  }

  std::unordered_map<Pointer, std::vector<Edge>> res;
  for (auto u : vertices) {
    std::unordered_map<Pointer, Edge> parent_edge;
    for (auto v : vertices) {
      if (u != v && dist[u][v] != INT_MAX) {
        Pointer curr = u;
        while (curr != v) {
          Pointer nxt = next[curr][v];
          if (!nxt)
            break;
          parent_edge.insert_or_assign(nxt, edge_map.at(curr).at(nxt));
          curr = nxt;
        }
      }
    }
    std::vector<Edge> spt;
    for (auto p : parent_edge) {
      spt.push_back(p.second);
    }
    res[u] = spt;
  }

  return res;
}

template <typename T>
std::unordered_map<typename Graph<T>::Pointer,
                   std::vector<typename Graph<T>::Edge>>
ShortestPathAllPairsJOHNSON(Graph<T> *graph) {
  return ShortestPathAllPairsFLOYD_WARSHALL(graph);
}

template <typename T>
std::vector<typename Graph<T>::Edge>
ShortestPathOneSourceFLOYD_WARSHALL(Graph<T> *graph, T *source) {

  return ShortestPathAllPairsFLOYD_WARSHALL(graph)[source];
}

template <typename T>
std::vector<typename Graph<T>::Edge>
ShortestPathOneSourceJOHNSON(Graph<T> *graph, T *source) {
  return ShortestPathAllPairsJOHNSON(graph)[source];
}

template <typename T>
std::vector<typename Graph<T>::Edge>
KshortestPathDirectedA_Star(Graph<T> *graph, typename Graph<T>::Pointer source,
                            typename Graph<T>::Pointer target, int k = 1) {
  using Pointer = typename Graph<T>::Pointer;
  using Edge = typename Graph<T>::Edge;
  struct Path {
    int cost;
    Pointer current_node;
    std::vector<Edge> edges;
    bool operator>(const Path &o) const { return cost > o.cost; }
  };
  std::priority_queue<Path, std::vector<Path>, std::greater<Path>> pq;
  pq.push({0, source, {}});
  int count = 0;
  std::vector<std::vector<Edge>> result;
  std::unordered_map<Pointer, int> visit_count;
  while (!pq.empty()) {
    auto p = pq.top();
    pq.pop();
    visit_count[p.current_node]++;
    if (p.current_node == target) {
      count++;
      if (count == k)
        return p.edges;
    }
    if (visit_count[p.current_node] > k * 2)
      continue;
    std::vector<Edge> out_edges;
    graph->getOutEdges(p.current_node, out_edges);
    for (const auto &e : out_edges) {
      Path next_p = p;
      next_p.cost += e.weight_;
      next_p.current_node = e.to.get_data();
      next_p.edges.push_back(e);
      pq.push(next_p);
    }
  }
  return {};
}
template <typename T>
std::vector<typename Graph<T>::Edge>
KshortestPathUnDirectedA_Star(Graph<T> *graph,
                              typename Graph<T>::Pointer source,
                              typename Graph<T>::Pointer target, int k = 1) {
  return KshortestPathDirectedA_Star(graph, source, target, k);
}
} // namespace
namespace graphTest {

enum ShortestPathAlgo { DIJKSTRA, BELLMAN_FORD, FLOYD_WARSHALL, JOHNSON };

template <typename T>
std::vector<typename Graph<T>::Edge>
ShortestPathOneSource(Graph<T> *graph, T *source, ShortestPathAlgo algo) {
  switch (algo) {
  case ShortestPathAlgo::DIJKSTRA:
    return ShortestPathOneSourceDIJKSTRA(graph, source);
  case ShortestPathAlgo::BELLMAN_FORD:
    return ShortestPathOneSourceBELLMAN_FORD(graph, source);
  case ShortestPathAlgo::FLOYD_WARSHALL:
    return ShortestPathOneSourceFLOYD_WARSHALL(graph, source);
  case ShortestPathAlgo::JOHNSON:
    return ShortestPathOneSourceJOHNSON(graph, source);
  default:
    std::cerr
        << "Invalid algorithm for one source shortest path. Valid options are "
           "DIJKSTRA, BELLMAN_FORD, FLOYD_WARSHALL, and JOHNSON."
        << std::endl;
    return {};
  }
  return {};
}

template <typename T>
std::unordered_map<typename Graph<T>::Pointer,
                   std::vector<typename Graph<T>::Edge>>
ShortestPathAllPairs(Graph<T> *graph, ShortestPathAlgo algo) {
  std::unordered_set<ShortestPathAlgo> valid_algos = {
      ShortestPathAlgo::FLOYD_WARSHALL, ShortestPathAlgo::JOHNSON};
  if (valid_algos.find(algo) == valid_algos.end()) {
    std::cerr << "Invalid algorithm for all pairs shortest path. Valid options "
                 "are FLOYD_WARSHALL and JOHNSON."
              << std::endl;
    return {};
  }

  if (algo == ShortestPathAlgo::FLOYD_WARSHALL) {
    return ShortestPathAllPairsFLOYD_WARSHALL(graph);
  } else {
    return ShortestPathAllPairsJOHNSON(graph);
  }
}

enum KShortestPathAlgo { A_STAR };
template <typename T>
std::vector<typename Graph<T>::Edge>
KshortestPath(Graph<T> *graph, typename Graph<T>::Pointer source,
              typename Graph<T>::Pointer target, KShortestPathAlgo algo,
              int k = 1) {
  if (!graph)
    return {};
  if (graph->isDirected()) {
    switch (algo) {
    case KShortestPathAlgo::A_STAR:
      return KshortestPathDirectedA_Star(graph, source, target, k);
    default:
      return {};
    }
  } else {
    switch (algo) {
    case KShortestPathAlgo::A_STAR:
      return KshortestPathUnDirectedA_Star(graph, source, target, k);
    default:
      return {};
    }
  }
}

} // namespace graphTest

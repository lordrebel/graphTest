#pragma once
#include "graph.hpp"
#include "graph_shartest_path.hpp"
#include <algorithm>
#include <climits>
#include <cstdint>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

namespace {
namespace gt = graphTest;

template <typename DataT> size_t countCircleThree(gt::Graph<DataT> *g) {
  using Pointer = typename gt::Graph<DataT>::Pointer;
  if (g->numVertexs() < 3) {
    return 0;
  }
  std::unordered_map<Pointer, std::unordered_map<Pointer, int>> adj;
  g->getAdjacencyMap(adj);
  auto vertexes = g->getAllVertexs();
  std::sort(vertexes.begin(), vertexes.end(), std::less<Pointer>());

  size_t count = 0;
  for (auto *u : vertexes) {
    auto uit = adj.find(u);
    if (uit == adj.end()) {
      continue;
    }
    for (const auto &uv : uit->second) {
      auto *v = uv.first;
      if (v == u) {
        continue;
      }
      if (!std::less<Pointer>()(u, v)) {
        continue;
      }
      auto vit = adj.find(v);
      if (vit == adj.end()) {
        continue;
      }
      for (const auto &vw : vit->second) {
        auto *w = vw.first;
        if (w == u || w == v) {
          continue;
        }
        if (!std::less<Pointer>()(v, w)) {
          continue;
        }
        if (uit->second.find(w) != uit->second.end()) {
          ++count;
        }
      }
    }
  }
  return count;
}

template <typename Pointer> struct PointerPair {
  Pointer first;
  Pointer second;
  bool operator==(const PointerPair &other) const {
    return first == other.first && second == other.second;
  }
};

template <typename Pointer> struct PointerPairHash {
  size_t operator()(const PointerPair<Pointer> &p) const {
    const auto h1 = std::hash<Pointer>()(p.first);
    const auto h2 = std::hash<Pointer>()(p.second);
    return h1 ^ (h2 << 1);
  }
};

template <typename DataT> size_t countCircleFour(gt::Graph<DataT> *g) {
  using Pointer = typename gt::Graph<DataT>::Pointer;
  if (g->numVertexs() < 4) {
    return 0;
  }
  std::unordered_map<Pointer, std::unordered_map<Pointer, int>> adj;
  g->getAdjacencyMap(adj);
  std::unordered_map<PointerPair<Pointer>, std::uint64_t,
                     PointerPairHash<Pointer>>
      pairCount;

  auto vertexes = g->getAllVertexs();
  for (auto *mid : vertexes) {
    auto mit = adj.find(mid);
    if (mit == adj.end()) {
      continue;
    }
    std::vector<Pointer> neighbors;
    neighbors.reserve(mit->second.size());
    for (const auto &nxt : mit->second) {
      auto *n = nxt.first;
      if (n != mid) {
        neighbors.push_back(n);
      }
    }
    std::sort(neighbors.begin(), neighbors.end(), std::less<Pointer>());

    for (size_t i = 0; i < neighbors.size(); ++i) {
      for (size_t j = i + 1; j < neighbors.size(); ++j) {
        auto *a = neighbors[i];
        auto *b = neighbors[j];
        if (a == b) {
          continue;
        }
        if (std::less<Pointer>()(b, a)) {
          std::swap(a, b);
        }
        pairCount[{a, b}]++;
      }
    }
  }

  std::uint64_t total = 0;
  for (const auto &kv : pairCount) {
    const std::uint64_t c = kv.second;
    if (c >= 2) {
      total += (c * (c - 1)) / 2;
    }
  }
  return static_cast<size_t>(total / 2);
}

template <typename DataT>
std::vector<typename gt::Graph<DataT>::Edge>
findMinistCircleDijstra(gt::Graph<DataT> *g) {
  using Pointer = typename gt::Graph<DataT>::Pointer;
  using Edge = typename gt::Graph<DataT>::Edge;
  if (g->numVertexs() < 3) {
    return {};
  }

  auto allEdges = g->getAllEdges();
  std::int64_t minWeight = std::numeric_limits<std::int64_t>::max();
  std::vector<Edge> bestCycle;

  for (const auto &e : allEdges) {
    Pointer u = e.from.get_data();
    Pointer v = e.to.get_data();
    std::int64_t uvWeight = e.weight_;

    // Temporarily remove edge (u, v)
    g->removeEdge(u, v);

    // Run Dijkstra from u to find shortest path tree
    auto spt = ShortestPathOneSourceDIJKSTRA(g, u);

    // Re-add edge (u, v)
    g->addEdge(u, v, uvWeight);

    // Build parent map from the shortest path tree
    std::unordered_map<Pointer, Pointer> parent;
    std::unordered_map<Pointer, int> edgeWeight;
    for (const auto &pe : spt) {
      Pointer from = pe.from.get_data();
      Pointer to = pe.to.get_data();
      parent[to] = from;
      edgeWeight[to] = pe.weight_;
    }

    // Check if v is reachable from u
    if (parent.find(v) == parent.end()) {
      continue;
    }

    // Reconstruct path from u to v
    std::vector<Edge> pathEdges;
    std::int64_t pathWeight = 0;
    Pointer cur = v;
    while (cur != u) {
      Pointer p = parent[cur];
      // Find the actual edge (p -> cur)
      for (const auto &pe : spt) {
        if (pe.from.get_data() == p && pe.to.get_data() == cur) {
          pathEdges.push_back(pe);
          break;
        }
      }
      pathWeight += edgeWeight[cur];
      cur = p;
    }
    std::reverse(pathEdges.begin(), pathEdges.end());

    std::int64_t totalWeight = pathWeight + uvWeight;
    if (totalWeight < minWeight) {
      minWeight = totalWeight;
      pathEdges.push_back(e);
      bestCycle = pathEdges;
    }
  }

  return bestCycle;
}

template <typename DataT>
std::vector<typename gt::Graph<DataT>::Edge>
findMinistCircleFloydWarshall(gt::Graph<DataT> *g) {
  using Pointer = typename gt::Graph<DataT>::Pointer;
  using Edge = typename gt::Graph<DataT>::Edge;
  const std::int64_t INF = std::numeric_limits<std::int64_t>::max() / 4;
  if (g->numVertexs() < 3) {
    return {};
  }

  auto vertices = g->getAllVertexs();
  size_t n = vertices.size();

  // Map vertex pointers to indices for easier handling
  std::unordered_map<Pointer, size_t> vertexToIdx;
  std::vector<Pointer> idxToVertex(n);
  for (size_t i = 0; i < n; ++i) {
    vertexToIdx[vertices[i]] = i;
    idxToVertex[i] = vertices[i];
  }

  // dist[i][j] = shortest path from i to j using vertices < k as intermediates
  std::vector<std::vector<std::int64_t>> dist(
      n, std::vector<std::int64_t>(n, INF));
  // next[i][j] = next vertex on path from i to j
  std::vector<std::vector<int>> next(n, std::vector<int>(n, -1));

  // Initialize with direct edges (both directions for undirected)
  auto allEdges = g->getAllEdges();
  for (const auto &e : allEdges) {
    size_t u = vertexToIdx[e.from.get_data()];
    size_t v = vertexToIdx[e.to.get_data()];
    std::int64_t w = e.weight_;
    dist[u][v] = w;
    dist[v][u] = w; // Undirected: edge exists both ways
    next[u][v] = v;
    next[v][u] = u;
  }

  // Also set self-distances
  for (size_t i = 0; i < n; ++i) {
    dist[i][i] = 0;
  }

  std::int64_t minWeight = INF;
  std::vector<Edge> bestCycle;

  // Standard Floyd-Warshall with minimum cycle detection
  for (size_t k = 0; k < n; ++k) {
    // Before updating dist with vertex k, check cycles that use k as the
    // highest-index vertex
    for (size_t i = 0; i < k; ++i) {
      for (size_t j = 0; j < k; ++j) {
        if (i == j)
          continue;
        // Check if edges (i,k) and (k,j) exist
        std::int64_t w_ik = INF;
        std::int64_t w_kj = INF;
        for (const auto &e : allEdges) {
          size_t u = vertexToIdx[e.from.get_data()];
          size_t v = vertexToIdx[e.to.get_data()];
          std::int64_t w = e.weight_;
          if (u == i && v == k)
            w_ik = w;
          if (u == k && v == j)
            w_kj = w;
          // Also check reverse (undirected)
          if (u == k && v == i)
            w_ik = w;
          if (u == j && v == k)
            w_kj = w;
        }
        if (w_ik == INF || w_kj == INF)
          continue;
        if (dist[i][j] == INF)
          continue;

        std::int64_t cycleWeight = dist[i][j] + w_ik + w_kj;
        if (cycleWeight < minWeight) {
          minWeight = cycleWeight;
          // Reconstruct cycle: i -> ... -> j -> k -> i
          bestCycle.clear();

          // Path from i to j
          int curr = static_cast<int>(i);
          while (curr != static_cast<int>(j)) {
            int nxt = next[curr][j];
            if (nxt < 0)
              break;
            // Find the edge (curr -> nxt)
            for (const auto &e : allEdges) {
              size_t u = vertexToIdx[e.from.get_data()];
              size_t v = vertexToIdx[e.to.get_data()];
              if ((u == static_cast<size_t>(curr) &&
                   v == static_cast<size_t>(nxt)) ||
                  (u == static_cast<size_t>(nxt) &&
                   v == static_cast<size_t>(curr))) {
                bestCycle.push_back(e);
                break;
              }
            }
            curr = nxt;
          }

          // Edge j -> k
          for (const auto &e : allEdges) {
            size_t u = vertexToIdx[e.from.get_data()];
            size_t v = vertexToIdx[e.to.get_data()];
            if ((u == j && v == k) || (u == k && v == j)) {
              bestCycle.push_back(e);
              break;
            }
          }

          // Edge k -> i
          for (const auto &e : allEdges) {
            size_t u = vertexToIdx[e.from.get_data()];
            size_t v = vertexToIdx[e.to.get_data()];
            if ((u == k && v == i) || (u == i && v == k)) {
              bestCycle.push_back(e);
              break;
            }
          }
        }
      }
    }

    // Now update dist with vertex k as intermediate
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        if (dist[i][k] != INF && dist[k][j] != INF) {
          if (dist[i][k] + dist[k][j] < dist[i][j]) {
            dist[i][j] = dist[i][k] + dist[k][j];
            next[i][j] = next[i][k];
          }
        }
      }
    }
  }

  return bestCycle;
}
} // namespace

namespace graphTest {

enum CIRCLE_EDGE {
  THREE_EDGE,
  FOUR_EDGE,
};

enum MINIST_CIRCLE_ALGO {
  MINIST_DIJSKSTRA,
  MINIST_FLOYD_WARSHALL,
};

template <typename DataT>
size_t countCircle(Graph<DataT> *g, CIRCLE_EDGE mode) {
  if (g == nullptr) {
    std::cerr << "graph is nullptr" << std::endl;
    return 0;
  }
  if (g->isDirected()) {
    std::cerr << "count circle only support undirected graph" << std::endl;
    return 0;
  }
  switch (mode) {
  case THREE_EDGE:
    return countCircleThree(g);
  case FOUR_EDGE:
    return countCircleFour(g);
  default:
    std::cerr << "invalid mode" << std::endl;
    return 0;
  }
}

template <typename DataT>
std::vector<typename Graph<DataT>::Edge>
findMinistCircle(Graph<DataT> *g, MINIST_CIRCLE_ALGO algo) {
  if (g == nullptr) {
    std::cerr << "graph is nullptr" << std::endl;
    return {};
  }
  if (g->isDirected()) {
    std::cerr << "find minist circle only support undirected graph"
              << std::endl;
    return {};
  }

  if (algo == MINIST_DIJSKSTRA) {
    auto edges = g->getAllEdges();
    for (const auto &e : edges) {
      if (e.weight_ < 0) {
        std::cerr
            << "DIJKSTRA-based minimum circle does not support negative weights"
            << std::endl;
        return {};
      }
    }
  }

  switch (algo) {
  case MINIST_DIJSKSTRA:
    return findMinistCircleDijstra(g);
  case MINIST_FLOYD_WARSHALL:
    return findMinistCircleFloydWarshall(g);
  default:
    std::cerr << "invalid algorithm" << std::endl;
    return {};
  }
}
} // namespace graphTest
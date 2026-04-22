#pragma once
#include "graph.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
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
} // namespace

namespace graphTest {

enum CIRCLE_EDGE {
  THREE_EDGE,
  FOUR_EDGE,
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
} // namespace graphTest
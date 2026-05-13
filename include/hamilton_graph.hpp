#pragma once
#include "graph.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>

namespace {
namespace gt = graphTest;

template <typename DataT> struct HamiltonAdj {
  using Pointer = typename gt::Graph<DataT>::Pointer;
  std::vector<Pointer> vertexs;
  std::unordered_map<Pointer, size_t> index_of;
  std::vector<std::vector<size_t>> nexts;
  std::vector<std::vector<size_t>> prevs;
  std::vector<std::vector<size_t>> undirected_nexts;
  std::vector<std::vector<char>> has_edge;
};

template <typename DataT>
bool hasAnyIncidentEdge(
    const gt::Graph<DataT> *graph,
    const std::vector<typename gt::Graph<DataT>::Pointer> &vertexs) {
  for (auto v : vertexs) {
    if (graph->getIndegrees(v) + graph->getOutdegrees(v) > 0) {
      return true;
    }
  }
  return false;
}

template <typename DataT>
HamiltonAdj<DataT> buildHamiltonAdj(const gt::Graph<DataT> *graph) {
  using Pointer = typename gt::Graph<DataT>::Pointer;
  HamiltonAdj<DataT> adj;
  adj.vertexs = graph->getAllVertexs();

  const size_t n = adj.vertexs.size();
  for (size_t i = 0; i < n; ++i) {
    adj.index_of[adj.vertexs[i]] = i;
  }

  adj.nexts.assign(n, {});
  adj.prevs.assign(n, {});
  adj.undirected_nexts.assign(n, {});
  adj.has_edge.assign(n, std::vector<char>(n, 0));

  auto *mutable_graph = const_cast<gt::Graph<DataT> *>(graph);
  for (size_t i = 0; i < n; ++i) {
    Pointer u = adj.vertexs[i];
    for (Pointer v : mutable_graph->getNext(u)) {
      auto it = adj.index_of.find(v);
      if (it == adj.index_of.end()) {
        continue;
      }
      const size_t j = it->second;
      if (!adj.has_edge[i][j]) {
        adj.has_edge[i][j] = 1;
        adj.nexts[i].push_back(j);
        adj.prevs[j].push_back(i);
      }
    }
  }

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      if (adj.has_edge[i][j] || adj.has_edge[j][i]) {
        adj.undirected_nexts[i].push_back(j);
      }
    }
  }
  return adj;
}

inline size_t dfsCountFrom(size_t start,
                           const std::vector<std::vector<size_t>> &nexts) {
  std::vector<char> visited(nexts.size(), 0);
  std::stack<size_t> st;
  st.push(start);
  visited[start] = 1;
  size_t count = 0;

  while (!st.empty()) {
    size_t cur = st.top();
    st.pop();
    ++count;
    for (size_t nxt : nexts[cur]) {
      if (!visited[nxt]) {
        visited[nxt] = 1;
        st.push(nxt);
      }
    }
  }
  return count;
}

template <typename DataT>
bool isConnectedUndirectedAll(const HamiltonAdj<DataT> &adj) {
  if (adj.vertexs.empty()) {
    return false;
  }
  return dfsCountFrom(0, adj.undirected_nexts) == adj.vertexs.size();
}

template <typename DataT>
bool isStronglyConnectedAll(const HamiltonAdj<DataT> &adj) {
  if (adj.vertexs.empty()) {
    return false;
  }
  return dfsCountFrom(0, adj.nexts) == adj.vertexs.size() &&
         dfsCountFrom(0, adj.prevs) == adj.vertexs.size();
}

template <typename DataT>
bool hasHamiltonianPathByBacktracking(const HamiltonAdj<DataT> &adj) {
  const size_t n = adj.vertexs.size();
  if (n == 0) {
    return false;
  }

  std::vector<char> visited(n, 0);
  std::function<bool(size_t, size_t)> dfs = [&](size_t cur,
                                                size_t used_count) -> bool {
    if (used_count == n) {
      return true;
    }
    for (size_t nxt : adj.nexts[cur]) {
      if (!visited[nxt]) {
        visited[nxt] = 1;
        if (dfs(nxt, used_count + 1)) {
          return true;
        }
        visited[nxt] = 0;
      }
    }
    return false;
  };

  for (size_t start = 0; start < n; ++start) {
    std::fill(visited.begin(), visited.end(), 0);
    visited[start] = 1;
    if (dfs(start, 1)) {
      return true;
    }
  }
  return false;
}

template <typename DataT>
bool hasHamiltonianCycleByBacktracking(const HamiltonAdj<DataT> &adj) {
  const size_t n = adj.vertexs.size();
  if (n == 0) {
    return false;
  }

  std::vector<char> visited(n, 0);
  std::function<bool(size_t, size_t, size_t)> dfs =
      [&](size_t start, size_t cur, size_t used_count) -> bool {
    if (used_count == n) {
      return adj.has_edge[cur][start] != 0;
    }
    for (size_t nxt : adj.nexts[cur]) {
      if (!visited[nxt]) {
        visited[nxt] = 1;
        if (dfs(start, nxt, used_count + 1)) {
          return true;
        }
        visited[nxt] = 0;
      }
    }
    return false;
  };

  for (size_t start = 0; start < n; ++start) {
    std::fill(visited.begin(), visited.end(), 0);
    visited[start] = 1;
    if (dfs(start, start, 1)) {
      return true;
    }
  }
  return false;
}

template <typename DataT>
bool isHamiltonianDirected(const gt::Graph<DataT> *graph) {
  auto adj = buildHamiltonAdj(graph);
  if (adj.vertexs.empty()) {
    return false;
  }
  if (!hasAnyIncidentEdge(graph, adj.vertexs)) {
    return false;
  }
  if (!isStronglyConnectedAll(adj)) {
    return false;
  }
  return hasHamiltonianCycleByBacktracking(adj);
}

template <typename DataT>
bool isHamiltonianUndirected(const gt::Graph<DataT> *graph) {
  auto adj = buildHamiltonAdj(graph);
  if (adj.vertexs.empty()) {
    return false;
  }
  if (!hasAnyIncidentEdge(graph, adj.vertexs)) {
    return false;
  }
  if (!isConnectedUndirectedAll(adj)) {
    return false;
  }
  return hasHamiltonianCycleByBacktracking(adj);
}

template <typename DataT>
bool isSemiHamiltonianDirected(const gt::Graph<DataT> *graph) {
  auto adj = buildHamiltonAdj(graph);
  if (adj.vertexs.empty()) {
    return false;
  }
  if (!hasAnyIncidentEdge(graph, adj.vertexs)) {
    return false;
  }
  if (!isConnectedUndirectedAll(adj)) {
    return false;
  }

  if (hasHamiltonianCycleByBacktracking(adj)) {
    return false;
  }
  return hasHamiltonianPathByBacktracking(adj);
}

template <typename DataT>
bool isSemiHamiltonianUndirected(const gt::Graph<DataT> *graph) {
  auto adj = buildHamiltonAdj(graph);
  if (adj.vertexs.empty()) {
    return false;
  }
  if (!hasAnyIncidentEdge(graph, adj.vertexs)) {
    return false;
  }
  if (!isConnectedUndirectedAll(adj)) {
    return false;
  }

  if (hasHamiltonianCycleByBacktracking(adj)) {
    return false;
  }
  return hasHamiltonianPathByBacktracking(adj);
}
} // namespace

namespace graphTest {
template <typename DataT> bool isHamiltonian(const Graph<DataT> *graph) {
  if (graph == nullptr) {
    std::cerr << "input graph is nullptr" << std::endl;
    return false;
  }
  if (graph->isDirected()) {
    return isHamiltonianDirected(graph);
  }
  return isHamiltonianUndirected(graph);
}

template <typename DataT> bool isSemiHamiltonian(const Graph<DataT> *graph) {
  if (graph == nullptr) {
    std::cerr << "input graph is nullptr" << std::endl;
    return false;
  }
  if (graph->isDirected()) {
    return isSemiHamiltonianDirected(graph);
  }
  return isSemiHamiltonianUndirected(graph);
}

} // namespace graphTest

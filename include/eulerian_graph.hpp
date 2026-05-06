#pragma once
#include "basicAlgo.hpp"
#include "graph.hpp"
#include <stack>
#include <unordered_set>
#include <vector>

namespace {
namespace gt = graphTest;

template <typename DataT>
std::vector<typename gt::Graph<DataT>::Pointer>
getNonZeroDegreeVertexs(const gt::Graph<DataT> *graph) {
  using Pointer = typename gt::Graph<DataT>::Pointer;
  std::vector<Pointer> non_zero;
  for (Pointer v : graph->getAllVertexs()) {
    const int in_deg = graph->getIndegrees(v);
    const int out_deg = graph->getOutdegrees(v);
    if (in_deg + out_deg > 0) {
      non_zero.push_back(v);
    }
  }
  return non_zero;
}

template <typename DataT>
bool isConnectedUndirectedOnNonZero(
    const gt::Graph<DataT> *graph,
    const std::vector<typename gt::Graph<DataT>::Pointer> &non_zero) {
  if (non_zero.empty()) {
    return false;
  }

  using Pointer = typename gt::Graph<DataT>::Pointer;
  std::unordered_set<Pointer> valid(non_zero.begin(), non_zero.end());
  std::unordered_set<Pointer> visited;
  std::stack<Pointer> st;
  auto *mutable_graph = const_cast<gt::Graph<DataT> *>(graph);

  st.push(non_zero.front());
  visited.insert(non_zero.front());
  while (!st.empty()) {
    Pointer cur = st.top();
    st.pop();
    for (Pointer nxt : mutable_graph->getNext(cur)) {
      if (valid.count(nxt) && !visited.count(nxt)) {
        visited.insert(nxt);
        st.push(nxt);
      }
    }
  }

  return visited.size() == non_zero.size();
}

template <typename DataT>
bool isStronglyConnectedOnNonZero(
    const gt::Graph<DataT> *graph,
    const std::vector<typename gt::Graph<DataT>::Pointer> &non_zero) {
  if (non_zero.empty()) {
    return false;
  }

  using Pointer = typename gt::Graph<DataT>::Pointer;
  std::unordered_set<Pointer> valid(non_zero.begin(), non_zero.end());
  auto *mutable_graph = const_cast<gt::Graph<DataT> *>(graph);

  auto run_dfs = [&](bool reversed) {
    std::unordered_set<Pointer> visited;
    std::stack<Pointer> st;
    st.push(non_zero.front());
    visited.insert(non_zero.front());

    while (!st.empty()) {
      Pointer cur = st.top();
      st.pop();
      std::vector<Pointer> nexts =
          reversed ? mutable_graph->getPrev(cur) : mutable_graph->getNext(cur);
      for (Pointer nxt : nexts) {
        if (valid.count(nxt) && !visited.count(nxt)) {
          visited.insert(nxt);
          st.push(nxt);
        }
      }
    }
    return visited.size() == non_zero.size();
  };

  return run_dfs(false) && run_dfs(true);
}

template <typename DataT>
bool isEulerianDirected(const gt::Graph<DataT> *graph) {
  auto non_zero = getNonZeroDegreeVertexs(graph);
  if (non_zero.empty()) {
    return false;
  }
  if (!isStronglyConnectedOnNonZero(graph, non_zero)) {
    return false;
  }

  for (auto v : graph->getAllVertexs()) {
    if (graph->getIndegrees(v) != graph->getOutdegrees(v)) {
      return false;
    }
  }
  return true;
}
template <typename DataT>
bool isEulerianUndirected(const gt::Graph<DataT> *graph) {
  auto non_zero = getNonZeroDegreeVertexs(graph);
  if (non_zero.empty()) {
    return false;
  }
  if (!isConnectedUndirectedOnNonZero(graph, non_zero)) {
    return false;
  }

  for (auto v : graph->getAllVertexs()) {
    if (graph->getIndegrees(v) % 2 != 0) {
      return false;
    }
  }
  return true;
}
template <typename DataT>
bool isSemiEulerianDirected(const gt::Graph<DataT> *graph) {
  auto non_zero = getNonZeroDegreeVertexs(graph);
  if (non_zero.empty()) {
    return false;
  }
  if (!isStronglyConnectedOnNonZero(graph, non_zero)) {
    return false;
  }

  int start_count = 0;
  int end_count = 0;
  for (auto v : graph->getAllVertexs()) {
    const int in_deg = graph->getIndegrees(v);
    const int out_deg = graph->getOutdegrees(v);
    const int diff = out_deg - in_deg;
    if (diff == 1) {
      ++start_count;
    } else if (diff == -1) {
      ++end_count;
    } else if (diff != 0) {
      return false;
    }
  }
  return start_count == 1 && end_count == 1;
}
template <typename DataT>
bool isSemiEulerianUndirected(const gt::Graph<DataT> *graph) {
  auto non_zero = getNonZeroDegreeVertexs(graph);
  if (non_zero.empty()) {
    return false;
  }
  if (!isConnectedUndirectedOnNonZero(graph, non_zero)) {
    return false;
  }

  int odd_count = 0;
  for (auto v : graph->getAllVertexs()) {
    if (graph->getIndegrees(v) % 2 != 0) {
      ++odd_count;
    }
  }
  return odd_count == 2;
}
} // namespace

namespace graphTest {
template <typename DataT> bool isEulerian(const Graph<DataT> *graph) {
  if (graph == nullptr) {
    std::cerr << "input graph is nullptr" << std::endl;
    return false;
  }
  if (graph->isDirected()) {
    return isEulerianDirected(graph);
  } else {
    return isEulerianUndirected(graph);
  }
}

template <typename DataT> bool isSemiEulerian(const Graph<DataT> *graph) {
  if (graph == nullptr) {
    std::cerr << "input graph is nullptr" << std::endl;
    return false;
  }
  if (graph->isDirected()) {
    return isSemiEulerianDirected(graph);
  } else {
    return isSemiEulerianUndirected(graph);
  }
}

} // namespace graphTest
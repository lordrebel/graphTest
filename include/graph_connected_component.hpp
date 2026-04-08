#pragma once
#include "graph.hpp"
#include <algorithm>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace graphTest {
enum SCCAlgo { Tarjan, Kosaraju };
}

namespace {
template <typename DataT>
void tarjanDFS(
    typename graphTest::Graph<DataT>::Pointer u,
    const graphTest::Graph<DataT> *graph, int &timer,
    std::unordered_map<typename graphTest::Graph<DataT>::Pointer, int> &dfn,
    std::unordered_map<typename graphTest::Graph<DataT>::Pointer, int> &low,
    std::stack<typename graphTest::Graph<DataT>::Pointer> &st,
    std::unordered_set<typename graphTest::Graph<DataT>::Pointer> &in_st,
    std::vector<std::vector<typename graphTest::Graph<DataT>::Pointer>> &sccs) {
  typedef typename graphTest::Graph<DataT>::Pointer Pointer;
  dfn[u] = low[u] = ++timer;
  st.push(u);
  in_st.insert(u);

  std::vector<typename graphTest::Graph<DataT>::Edge> outEdges;
  graph->getOutEdges(u, outEdges);
  for (const auto &edge : outEdges) {
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
      if (v == u)
        break;
    }
    sccs.push_back(component);
  }
}

template <typename DataT>
std::vector<std::vector<typename graphTest::Graph<DataT>::Pointer>>
getSCCTarjan(const graphTest::Graph<DataT> *graph) {
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

template <typename DataT>
void kosarajuDFS1(
    typename graphTest::Graph<DataT>::Pointer u,
    const graphTest::Graph<DataT> *graph,
    std::unordered_set<typename graphTest::Graph<DataT>::Pointer> &visited,
    std::vector<typename graphTest::Graph<DataT>::Pointer> &order) {
  typedef typename graphTest::Graph<DataT>::Pointer Pointer;
  visited.insert(u);
  std::vector<typename graphTest::Graph<DataT>::Edge> outEdges;
  graph->getOutEdges(u, outEdges);
  for (const auto &edge : outEdges) {
    Pointer v = edge.to.get_data();
    if (visited.find(v) == visited.end()) {
      kosarajuDFS1<DataT>(v, graph, visited, order);
    }
  }
  order.push_back(u);
}

template <typename DataT>
void kosarajuDFS2(
    typename graphTest::Graph<DataT>::Pointer u,
    const graphTest::Graph<DataT> *graph,
    std::unordered_set<typename graphTest::Graph<DataT>::Pointer> &visited,
    std::vector<typename graphTest::Graph<DataT>::Pointer> &component) {
  typedef typename graphTest::Graph<DataT>::Pointer Pointer;
  visited.insert(u);
  component.push_back(u);
  std::vector<typename graphTest::Graph<DataT>::Edge> inEdges;
  graph->getInEdges(u, inEdges);
  for (const auto &edge : inEdges) {
    Pointer v = edge.from.get_data();
    if (visited.find(v) == visited.end()) {
      kosarajuDFS2<DataT>(v, graph, visited, component);
    }
  }
}

template <typename DataT>
std::vector<std::vector<typename graphTest::Graph<DataT>::Pointer>>
getSCCKosaraju(const graphTest::Graph<DataT> *graph) {
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

template <typename DataT>
void tarjanVBCCDFS(
    typename graphTest::Graph<DataT>::Pointer u,
    typename graphTest::Graph<DataT>::Pointer p,
    const graphTest::Graph<DataT> *graph, int &timer,
    std::unordered_map<typename graphTest::Graph<DataT>::Pointer, int> &dfn,
    std::unordered_map<typename graphTest::Graph<DataT>::Pointer, int> &low,
    std::stack<typename graphTest::Graph<DataT>::Pointer> &st,
    std::vector<std::vector<typename graphTest::Graph<DataT>::Pointer>> &vbccs) {
  typedef typename graphTest::Graph<DataT>::Pointer Pointer;
  dfn[u] = low[u] = ++timer;
  st.push(u);

  std::vector<typename graphTest::Graph<DataT>::Edge> outEdges;
  graph->getOutEdges(u, outEdges);
  int children = 0;
  for (const auto &edge : outEdges) {
    Pointer v = edge.to.get_data();
    if (v == p) continue;
    if (dfn.find(v) == dfn.end()) {
      children++;
      tarjanVBCCDFS<DataT>(v, u, graph, timer, dfn, low, st, vbccs);
      low[u] = std::min(low[u], low[v]);
      if (low[v] >= dfn[u]) {
        std::vector<Pointer> component;
        while (true) {
          Pointer top = st.top();
          st.pop();
          component.push_back(top);
          if (top == v) break;
        }
        component.push_back(u);
        vbccs.push_back(component);
      }
    } else {
      low[u] = std::min(low[u], dfn[v]);
    }
  }
  if (p == nullptr && children == 0) {
    vbccs.push_back({u});
  }
}

template <typename DataT>
void tarjanEBCCDFS(
    typename graphTest::Graph<DataT>::Pointer u,
    typename graphTest::Graph<DataT>::Pointer p,
    const graphTest::Graph<DataT> *graph, int &timer,
    std::unordered_map<typename graphTest::Graph<DataT>::Pointer, int> &dfn,
    std::unordered_map<typename graphTest::Graph<DataT>::Pointer, int> &low,
    std::stack<typename graphTest::Graph<DataT>::Pointer> &st,
    std::vector<std::vector<typename graphTest::Graph<DataT>::Pointer>> &ebccs) {
  typedef typename graphTest::Graph<DataT>::Pointer Pointer;
  dfn[u] = low[u] = ++timer;
  st.push(u);

  std::vector<typename graphTest::Graph<DataT>::Edge> outEdges;
  graph->getOutEdges(u, outEdges);
  for (const auto &edge : outEdges) {
    Pointer v = edge.to.get_data();
    if (v == p) continue;
    if (dfn.find(v) == dfn.end()) {
      tarjanEBCCDFS<DataT>(v, u, graph, timer, dfn, low, st, ebccs);
      low[u] = std::min(low[u], low[v]);
    } else {
      low[u] = std::min(low[u], dfn[v]);
    }
  }

  if (dfn[u] == low[u]) {
    std::vector<Pointer> component;
    while (true) {
      Pointer v = st.top();
      st.pop();
      component.push_back(v);
      if (v == u) break;
    }
    ebccs.push_back(component);
  }
}

} // namespace

namespace graphTest {
template <typename DataT>
std::vector<std::vector<typename Graph<DataT>::Pointer>>
getSCC(const Graph<DataT> *graph, SCCAlgo algo) {
  if (!graph) {
    std::cerr << "Graph is null pointer" << std::endl;
    return {};
  } else if (!graph->isDirected()) {
    std::cerr << "Graph is not directed" << std::endl;
    return {};
  } else {
    switch (algo) {
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

template <typename DataT>
std::vector<std::vector<typename Graph<DataT>::Pointer>>
getBiConnectedVertexCC(const Graph<DataT> *graph) {
  if (!graph) {
    std::cerr << "Graph is null pointer" << std::endl;
    return {};
  } else if (graph->isDirected()) {
    std::cerr << "Graph is directed biConnected CC is for undirected graph"
              << std::endl;
    return {};
  } else {
    typedef typename Graph<DataT>::Pointer Pointer;
    std::vector<std::vector<Pointer>> vbccs;
    std::unordered_map<Pointer, int> dfn, low;
    std::stack<Pointer> st;
    int timer = 0;
    for (Pointer v : graph->getAllVertexs()) {
      if (dfn.find(v) == dfn.end())
        tarjanVBCCDFS<DataT>(v, nullptr, graph, timer, dfn, low, st, vbccs);
    }
    return vbccs;
  }
}

template <typename DataT>
std::vector<std::vector<typename Graph<DataT>::Pointer>>
getBiConnectedEdgeCC(const Graph<DataT> *graph) {
  if (!graph) {
    std::cerr << "Graph is null pointer" << std::endl;
    return {};
  } else if (graph->isDirected()) {
    std::cerr << "Graph is directed biConnected CC is for undirected graph"
              << std::endl;
    return {};
  } else {
    typedef typename Graph<DataT>::Pointer Pointer;
    std::vector<std::vector<Pointer>> ebccs;
    std::unordered_map<Pointer, int> dfn, low;
    std::stack<Pointer> st;
    int timer = 0;
    for (Pointer v : graph->getAllVertexs()) {
      if (dfn.find(v) == dfn.end())
        tarjanEBCCDFS<DataT>(v, nullptr, graph, timer, dfn, low, st, ebccs);
    }
    return ebccs;
  }
}

} // namespace graphTest

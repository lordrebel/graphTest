#pragma once
#include "graph.hpp"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace graphTest {
enum SCCAlgo { Tarjan, Kosaraju };

template <typename DataT> struct BlockForestResult {
  typedef typename Graph<DataT>::Pointer Pointer;
  enum class NodeType { Round, Square };

  struct Node {
    NodeType type = NodeType::Round;
    Pointer vertex = nullptr;
    std::size_t block_id = 0;
  };

  struct Tree {
    std::vector<Node> nodes;
    std::vector<std::pair<std::size_t, std::size_t>> edges;
  };

  std::vector<Tree> trees;
};
} // namespace graphTest

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
    std::vector<std::vector<typename graphTest::Graph<DataT>::Pointer>>
        &vbccs) {
  typedef typename graphTest::Graph<DataT>::Pointer Pointer;
  dfn[u] = low[u] = ++timer;
  st.push(u);

  std::vector<typename graphTest::Graph<DataT>::Edge> outEdges;
  graph->getOutEdges(u, outEdges);
  int children = 0;
  for (const auto &edge : outEdges) {
    Pointer v = edge.to.get_data();
    if (v == p)
      continue;
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
          if (top == v)
            break;
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
    std::vector<std::vector<typename graphTest::Graph<DataT>::Pointer>>
        &ebccs) {
  typedef typename graphTest::Graph<DataT>::Pointer Pointer;
  dfn[u] = low[u] = ++timer;
  st.push(u);

  std::vector<typename graphTest::Graph<DataT>::Edge> outEdges;
  graph->getOutEdges(u, outEdges);
  for (const auto &edge : outEdges) {
    Pointer v = edge.to.get_data();
    if (v == p)
      continue;
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
      if (v == u)
        break;
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
template <typename DataT>
BlockForestResult<DataT> getBlockForest(const Graph<DataT> *graph) {
  if (!graph) {
    std::cerr << "Graph is null pointer" << std::endl;
    return {};
  } else if (graph->isDirected()) {
    std::cerr << "Graph is directed block forest is for undirected graph"
              << std::endl;
    return {};
  } else {
    typedef typename Graph<DataT>::Pointer Pointer;
    BlockForestResult<DataT> result;

    const std::vector<Pointer> vertices = graph->getAllVertexs();
    const std::vector<std::vector<Pointer>> vbccs =
        getBiConnectedVertexCC(graph);

    const std::size_t round_count = vertices.size();
    const std::size_t square_count = vbccs.size();
    const std::size_t total_nodes = round_count + square_count;

    std::unordered_map<Pointer, std::size_t> round_index;
    round_index.reserve(round_count);
    for (std::size_t i = 0; i < round_count; ++i) {
      round_index[vertices[i]] = i;
    }

    std::vector<typename BlockForestResult<DataT>::Node> global_nodes(
        total_nodes);
    for (std::size_t i = 0; i < round_count; ++i) {
      global_nodes[i].type = BlockForestResult<DataT>::NodeType::Round;
      global_nodes[i].vertex = vertices[i];
      global_nodes[i].block_id = 0;
    }
    for (std::size_t i = 0; i < square_count; ++i) {
      const std::size_t idx = round_count + i;
      global_nodes[idx].type = BlockForestResult<DataT>::NodeType::Square;
      global_nodes[idx].vertex = nullptr;
      global_nodes[idx].block_id = i;
    }

    std::vector<std::vector<std::size_t>> adj(total_nodes);
    for (std::size_t block_id = 0; block_id < square_count; ++block_id) {
      const std::size_t square_idx = round_count + block_id;
      std::unordered_set<Pointer> unique_vertices;
      for (Pointer v : vbccs[block_id]) {
        unique_vertices.insert(v);
      }
      for (Pointer v : unique_vertices) {
        auto it = round_index.find(v);
        if (it == round_index.end()) {
          continue;
        }
        const std::size_t round_idx = it->second;
        adj[round_idx].push_back(square_idx);
        adj[square_idx].push_back(round_idx);
      }
    }

    std::vector<bool> visited(total_nodes, false);
    for (std::size_t start = 0; start < total_nodes; ++start) {
      if (visited[start]) {
        continue;
      }

      std::queue<std::size_t> q;
      q.push(start);
      visited[start] = true;

      std::vector<std::size_t> component_nodes;
      while (!q.empty()) {
        const std::size_t cur = q.front();
        q.pop();
        component_nodes.push_back(cur);

        for (std::size_t nxt : adj[cur]) {
          if (!visited[nxt]) {
            visited[nxt] = true;
            q.push(nxt);
          }
        }
      }

      typename BlockForestResult<DataT>::Tree tree;
      std::unordered_map<std::size_t, std::size_t> local_index;
      local_index.reserve(component_nodes.size());
      for (std::size_t i = 0; i < component_nodes.size(); ++i) {
        const std::size_t global_idx = component_nodes[i];
        local_index[global_idx] = i;
        tree.nodes.push_back(global_nodes[global_idx]);
      }

      for (std::size_t global_u : component_nodes) {
        auto local_u_it = local_index.find(global_u);
        if (local_u_it == local_index.end()) {
          continue;
        }
        const std::size_t local_u = local_u_it->second;
        for (std::size_t global_v : adj[global_u]) {
          if (global_u >= global_v) {
            continue;
          }
          auto local_v_it = local_index.find(global_v);
          if (local_v_it == local_index.end()) {
            continue;
          }
          tree.edges.push_back({local_u, local_v_it->second});
        }
      }

      result.trees.push_back(std::move(tree));
    }

    return result;
  }
}

} // namespace graphTest

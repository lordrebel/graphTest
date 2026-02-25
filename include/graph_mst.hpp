
#include "basicAlgo.hpp"
#include "graph.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <climits>

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
  std::unordered_map<typename Graph<T>::Pointer,
                     std::vector<std::pair<typename Graph<T>::Pointer, int>>>
      edge_map;
  for (auto &e : edges) {
    uf.add(e.from.get_data());
    uf.add(e.to.get_data());
    if (uf.findParent(e.from.get_data()) != uf.findParent(e.to.get_data())) {
      uf.merge(e.from.get_data(), e.to.get_data());
      edge_map[e.from.get_data()].push_back({e.to.get_data(), e.weight_});
      edge_map[e.to.get_data()].push_back({e.from.get_data(), e.weight_});
    }
  }
  auto groups = uf.getGroups();
  std::vector<std::unique_ptr<Graph<T>>> mst_graphs;
  for (const auto &pair : groups) {
    const auto &group_nodes = pair.second;
    std::unordered_set<T *> node_set(group_nodes.begin(), group_nodes.end());
    auto mst_graph = std::make_unique<UnDirectedGraph<T>>();
    for (const auto &node : group_nodes) {
      for (auto p : edge_map[node]) {
        if (node_set.count(p.first)) {
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
  std::unordered_set<T *> unvisited;
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
    auto cmp = [](const EdgeType &a, const EdgeType &b) {
      return a.weight_ > b.weight_;
    };
    std::priority_queue<EdgeType, std::vector<EdgeType>, decltype(cmp)> edge_pq(
        cmp);
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

template <typename T>
std::vector<std::unique_ptr<Graph<T>>> getDMSTsZhuLiu(const Graph<T> *graph) {
    using Vertex = typename Graph<T>::Pointer;
    auto vs = graph->getAllVertexs();
    if (vs.empty()) return {};

    int n = vs.size();
    
    // 1. 结构准备：记录 u, v, w 以及它们对应的原始顶点索引
    struct InternalEdge {
        int u, v, w;
        int orig_u, orig_v;
    };

    std::vector<InternalEdge> edges;
    std::unordered_map<Vertex, int> v2id;
    for (int i = 0; i < n; ++i) v2id[vs[i]] = i;

    std::unordered_map<Vertex, std::unordered_map<Vertex, int>> adj;
    graph->getAdjacencyMap(adj);

    long long weight_sum = 0;
    for (const auto& u_pair : adj) {
        int u = v2id[u_pair.first];
        for (const auto& v_pair : u_pair.second) {
            int v = v2id[v_pair.first];
            if (u == v) continue;
            edges.push_back({u, v, v_pair.second, u, v});
            weight_sum += std::abs(v_pair.second);
        }
    }

    // 2. 超级虚拟根：ID 设为 n，建立全连通
    int virtual_root = n;
    int INF_W = static_cast<int>(weight_sum + 7); 
    for (int i = 0; i < n; ++i) {
        edges.push_back({virtual_root, i, INF_W, virtual_root, i});
    }

    struct FinalEdge { int u, v, w; };

    // 3. 朱-刘缩点递归
    auto solve = [&](auto self, int num_v, int curr_root, std::vector<InternalEdge> curr_edges) -> std::vector<FinalEdge> {
        std::vector<int> min_in(num_v, INT_MAX);
        std::vector<int> pre(num_v, -1);
        std::vector<InternalEdge> min_edge_info(num_v);

        // 选最小入边
        for (const auto& e : curr_edges) {
            if (e.u != e.v && e.v != curr_root && e.w < min_in[e.v]) {
                min_in[e.v] = e.w;
                pre[e.v] = e.u;
                min_edge_info[e.v] = e;
            }
        }

        // 找环逻辑
        std::vector<int> id(num_v, -1);
        std::vector<int> visit(num_v, -1);
        int group_cnt = 0;

        for (int i = 0; i < num_v; ++i) {
            if (i == curr_root) continue;
            int v = i;
            while (v != curr_root && visit[v] != i && id[v] == -1) {
                visit[v] = i;
                v = pre[v];
            }
            if (v != curr_root && id[v] == -1 && visit[v] == i) {
                int start = v;
                do {
                    id[v] = group_cnt;
                    v = pre[v];
                } while (v != start);
                group_cnt++;
            }
        }

        // 无环了，开始回档
        if (group_cnt == 0) {
            std::vector<FinalEdge> res;
            for (int i = 0; i < num_v; ++i) {
                if (i == curr_root) continue;
                int u_orig = min_edge_info[i].orig_u;
                int v_orig = min_edge_info[i].orig_v;
                // 虚拟边权重处理
                int weight = (u_orig == virtual_root) ? INF_W : adj[vs[u_orig]][vs[v_orig]];
                res.push_back({u_orig, v_orig, weight});
            }
            return res;
        }

        for (int i = 0; i < num_v; ++i) if (id[i] == -1) id[i] = group_cnt++;

        // 缩点更新权限
        std::vector<InternalEdge> next_edges;
        for (const auto& e : curr_edges) {
            int u = id[e.u], v = id[e.v];
            if (u != v) {
                next_edges.push_back({u, v, e.w - min_in[e.v], e.orig_u, e.orig_v});
            }
        }

        auto sub_res = self(self, group_cnt, id[curr_root], next_edges);
        
        // 关键：还原被挤掉的入边
        std::vector<bool> covered(num_v, false);
        std::vector<FinalEdge> final_res;
        for (const auto& e : sub_res) {
            final_res.push_back(e);
            for (const auto& ce : curr_edges) {
                if (ce.orig_u == e.u && ce.orig_v == e.v) {
                    covered[ce.v] = true;
                    break;
                }
            }
        }
        for (int i = 0; i < num_v; ++i) {
            if (i != curr_root && !covered[i]) {
                int u_orig = min_edge_info[i].orig_u;
                int v_orig = min_edge_info[i].orig_v;
                int weight = (u_orig == virtual_root) ? INF_W : adj[vs[u_orig]][vs[v_orig]];
                final_res.push_back({u_orig, v_orig, weight});
            }
        }
        return final_res;
    };

    // 4. 计算结果并修剪
    auto all_edges = solve(solve, n + 1, virtual_root, edges);

    std::vector<FinalEdge> forest_edges;
    for (auto& e : all_edges) {
        // 去掉从虚拟上帝节点出来的边
        if (e.u != virtual_root) forest_edges.push_back(e);
    }

    // 5. 【修正】UnionFind 正确姿势：传 T* 指针
    UnionFind<T> uf;
    for (int i = 0; i < n; ++i) uf.add(vs[i]); 
    for (auto& e : forest_edges) uf.merge(vs[e.u], vs[e.v]); 

    auto groups = uf.getGroups(); // map<T*, vector<T*>>
    std::vector<std::unique_ptr<Graph<T>>> results;

    for (auto& pair : groups) {
        auto dmst = std::make_unique<DirectedGraph<T>>();
        std::unordered_set<Vertex> group_nodes(pair.second.begin(), pair.second.end());
        
        for (auto& e : forest_edges) {
            if (group_nodes.count(vs[e.v])) { 
                dmst->addEdge(vs[e.u], vs[e.v], e.w);
            }
        }
        // 保证孤立点也在里面
        for (Vertex node : pair.second) dmst->addVertex(node);
        results.push_back(std::move(dmst));
    }

    return results;
}
template <typename T>
std::vector<std::unique_ptr<Graph<T>>> getDMSTsTarjan(const Graph<T> *graph) {
  // TODO
  return {};
}

} // namespace
enum MSTAlgo { KRUSKAL, PRIM };
//If the graph is not connected, this function returns a Minimum Spanning Forest (MSF) represented as a collection of MSTs for each connected component.
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

enum DMSTAlgo { ZHULIU, TARJAN };
template <typename T>
std::vector<std::unique_ptr<Graph<T>>> getDMSTs(const Graph<T> *graph,
                                                DMSTAlgo algo) {
  if (!graph->isDirected()) {
    std::cerr << "DMST only applies to directed graph" << std::endl;
    return {};
  }
  switch (algo) {
  case ZHULIU:
    return getDMSTsZhuLiu<T>(graph);
  case TARJAN:
    return getDMSTsTarjan<T>(graph);
  default:
    std::cerr << "unknown DMST algo" << std::endl;
    return {};
  }
}

} // namespace graphTest
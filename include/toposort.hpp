#include "graph.hpp"
#include "graph_check.hpp"
#include "basicAlgo.hpp"
#include <iostream>
#include <queue>
namespace graphTest {
enum class TopoSortAlgo { Kahn, RPO };
namespace {
template <typename T>
std::vector<typename Graph<T>::Pointer>
TopologicalSortKahn(Graph<T> *g, bool reverse = false) {
  std::queue<typename Graph<T>::Pointer> q;
  std::unordered_set<typename Graph<T>::Pointer> visited;
  std::vector<typename Graph<T>::Pointer> order;
  std::unordered_map<typename Graph<T>::Pointer, int> degrees;
  for (const auto &vertex : g->getAllVertexs()) {
    if (!reverse) {
      degrees[vertex] = g->getIndegrees(vertex);
      if (degrees[vertex] == 0) {
        q.push(vertex);
        visited.insert(vertex);
      }
    } else {
      degrees[vertex] = g->getOutdegrees(vertex);
      if (degrees[vertex] == 0) {
        q.push(vertex);
        visited.insert(vertex);
      }
    }
  }
  while (!q.empty()) {
    auto vertex = q.front();
    q.pop();
    order.push_back(vertex);
    for (const auto &next : reverse ? g->getPrev(vertex) : g->getNext(vertex)) {
      if (visited.count(next) == 0) {
        degrees[next]--;
        if (degrees[next] == 0) {
          q.push(next);
          visited.insert(next);
        }
      }
    }
  }
  return order;
}

template <typename T>
std::vector<typename Graph<T>::Pointer>
TopologicalSortRPO(Graph<T> *g, bool reverse = false) {
  std::vector<typename Graph<T>::Pointer> postOrder;
  auto postVisit = [&](typename Graph<T>::Pointer v) {
    postOrder.push_back(v);
  };

  std::function<std::vector<typename Graph<T>::Pointer>(
      typename Graph<T>::Pointer)>
      getNexts = [g, reverse](typename Graph<T>::Pointer data) {
        return reverse ? g->getPrev(data) : g->getNext(data);
      };
  auto allVertexs = g->getAllVertexs();
  DFS<T>(allVertexs, getNexts, nullptr, postVisit);
  std::reverse(postOrder.begin(), postOrder.end());
  return postOrder;
}

} // namespace

template <typename T>
std::vector<typename Graph<T>::Pointer>
TopologicalSort(Graph<T> *g, bool reverse = false,
                TopoSortAlgo algo = TopoSortAlgo::Kahn) {
  if (g == nullptr) {
    std::cerr << "nullptr input graph\n";
    return {};
  }
  if (!isDAG(g)) {
    std::cerr << "Input graph is not a DAG,cannot toposort\n";
    return {};
  }
  if (algo == TopoSortAlgo::Kahn) {
    return TopologicalSortKahn(g, reverse);
  } else if (algo == TopoSortAlgo::RPO) {
    return TopologicalSortRPO(g, reverse);
  } else {
    std::cerr << "Unknown toposort algorithm\n";
    return {};
  }
}

} // namespace graphTest
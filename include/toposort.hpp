#include "graph.hpp"
#include "graph_check.hpp"
#include <iostream>
#include <queue>
namespace graphTest {
namespace {
template <typename T>
std::vector<typename Graph<T>::Pointer>
TopologicalSortDAG(Graph<T> *g, bool reverse = false) {
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

} // namespace

template <typename T>
std::vector<typename Graph<T>::Pointer> TopologicalSort(Graph<T> *g,
                                                        bool reverse = false) {
  if (g == nullptr) {
    std::cerr << "nullptr input graph\n";
    return {};
  }
  if (!isDAG(g)) {
    std::cerr << "Input graph is not a DAG,cannot toposort\n";
    return {};
  }
  return TopologicalSortDAG(g, reverse);
}

} // namespace graphTest
#include "graph_circle_count.hpp"
#include <cassert>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

namespace gt = graphTest;

#define TEST_AND_RUN(test_name)                                                \
  std::cout << "Running " << #test_name << "..." << std::endl;                 \
  test_name();                                                                 \
  std::cout << #test_name << " passed!" << std::endl;

template <typename T>
int totalWeight(const std::vector<typename gt::Graph<T>::Edge> &edges) {
  int sum = 0;
  for (const auto &e : edges) {
    sum += e.weight_;
  }
  return sum;
}

template <typename T>
bool isSimpleCycle(const std::vector<typename gt::Graph<T>::Edge> &edges) {
  using Pointer = typename gt::Graph<T>::Pointer;
  if (edges.size() < 3) {
    return false;
  }

  std::unordered_map<Pointer, int> degree;
  std::unordered_map<Pointer, std::vector<Pointer>> adj;
  for (const auto &e : edges) {
    Pointer u = e.from.get_data();
    Pointer v = e.to.get_data();
    if (u == v) {
      return false;
    }
    degree[u]++;
    degree[v]++;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  if (degree.size() != edges.size()) {
    return false;
  }

  for (const auto &kv : degree) {
    if (kv.second != 2) {
      return false;
    }
  }

  std::unordered_map<Pointer, bool> visited;
  std::queue<Pointer> q;
  Pointer start = edges.front().from.get_data();
  visited[start] = true;
  q.push(start);

  while (!q.empty()) {
    Pointer u = q.front();
    q.pop();
    for (Pointer v : adj[u]) {
      if (!visited[v]) {
        visited[v] = true;
        q.push(v);
      }
    }
  }

  return visited.size() == degree.size();
}

void test_null_and_small_graph() {
  gt::Graph<int> *nullGraph = nullptr;
  assert(gt::countCircle(nullGraph, gt::THREE_EDGE) == 0);
  assert(gt::countCircle(nullGraph, gt::FOUR_EDGE) == 0);

  std::vector<int> data = {1, 2, 3};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1]);
  assert(gt::countCircle(&g, gt::THREE_EDGE) == 0);
  assert(gt::countCircle(&g, gt::FOUR_EDGE) == 0);
}

void test_triangle_only() {
  std::vector<int> data = {1, 2, 3};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[1], &data[2]);
  g.addEdge(&data[2], &data[0]);

  assert(gt::countCircle(&g, gt::THREE_EDGE) == 1);
  assert(gt::countCircle(&g, gt::FOUR_EDGE) == 0);
}

void test_square_with_diagonal() {
  std::vector<int> data = {1, 2, 3, 4};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[1], &data[2]);
  g.addEdge(&data[2], &data[3]);
  g.addEdge(&data[3], &data[0]);
  g.addEdge(&data[0], &data[2]);

  assert(gt::countCircle(&g, gt::THREE_EDGE) == 2);
  assert(gt::countCircle(&g, gt::FOUR_EDGE) == 1);
}

void test_k4_complete_graph() {
  std::vector<int> data = {1, 2, 3, 4};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[0], &data[2]);
  g.addEdge(&data[0], &data[3]);
  g.addEdge(&data[1], &data[2]);
  g.addEdge(&data[1], &data[3]);
  g.addEdge(&data[2], &data[3]);

  assert(gt::countCircle(&g, gt::THREE_EDGE) == 4);
  assert(gt::countCircle(&g, gt::FOUR_EDGE) == 3);
}

void test_directed_graph_guard() {
  std::vector<int> data = {1, 2, 3, 4};
  gt::DirectedGraph<int> g;
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[1], &data[2]);
  g.addEdge(&data[2], &data[3]);
  g.addEdge(&data[3], &data[0]);

  assert(gt::countCircle(&g, gt::THREE_EDGE) == 0);
  assert(gt::countCircle(&g, gt::FOUR_EDGE) == 0);
}

// ========== Minimum Circle Tests (Dijkstra-based) ==========

void test_minist_circle_dijkstra_triangle() {
  // Triangle: 1-2-3-1, all weight 1
  // The only cycle is the triangle itself, total weight = 3
  std::vector<int> data = {1, 2, 3};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1], 1);
  g.addEdge(&data[1], &data[2], 1);
  g.addEdge(&data[2], &data[0], 1);

  auto cycle = gt::findMinistCircle(&g, gt::MINIST_DIJSKSTRA);
  assert(!cycle.empty());
  assert(isSimpleCycle<int>(cycle));
  assert(totalWeight<int>(cycle) == 3);
  assert(cycle.size() == 3); // 3 edges in a triangle
}

void test_minist_circle_dijkstra_square() {
  // Square: 1-2-3-4-1, all weight 1
  // The only cycle is the square itself, total weight = 4
  std::vector<int> data = {1, 2, 3, 4};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1], 1);
  g.addEdge(&data[1], &data[2], 1);
  g.addEdge(&data[2], &data[3], 1);
  g.addEdge(&data[3], &data[0], 1);

  auto cycle = gt::findMinistCircle(&g, gt::MINIST_DIJSKSTRA);
  assert(!cycle.empty());
  assert(isSimpleCycle<int>(cycle));
  assert(totalWeight<int>(cycle) == 4);
}

void test_minist_circle_dijkstra_weighted() {
  // Graph:
  //   1 --1-- 2
  //   |       |
  //   2       1
  //   |       |
  //   3 --2-- 4
  //   |       |
  //   1       1
  //   |       |
  //   5 --1-- 6
  // Cycles:
  //   1-2-4-3-1: weight = 1+1+2+2 = 6
  //   3-4-6-5-3: weight = 2+1+1+1 = 5 (minimum)
  std::vector<int> data = {1, 2, 3, 4, 5, 6};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1], 1); // 1-2
  g.addEdge(&data[1], &data[3], 1); // 2-4
  g.addEdge(&data[3], &data[2], 2); // 4-3
  g.addEdge(&data[2], &data[0], 2); // 3-1
  g.addEdge(&data[2], &data[4], 1); // 3-5
  g.addEdge(&data[4], &data[5], 1); // 5-6
  g.addEdge(&data[5], &data[3], 1); // 6-4

  auto cycle = gt::findMinistCircle(&g, gt::MINIST_DIJSKSTRA);
  assert(!cycle.empty());
  assert(isSimpleCycle<int>(cycle));
  assert(totalWeight<int>(cycle) == 5); // minimum cycle weight = 5
}

void test_minist_circle_dijkstra_complex() {
  // Graph with multiple cycles:
  //   1 --10-- 2
  //   |        |
  //   5        3
  //   |        |
  //   4 --1-- 3
  //   |        |
  //   2        4
  //   |        |
  //   5 --2-- 6
  // Cycles:
  //   1-2-3-4-1: weight = 10+3+1+5 = 19
  //   4-3-6-5-4: weight = 1+4+2+2 = 9 (minimum)
  std::vector<int> data = {1, 2, 3, 4, 5, 6};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1], 10); // 1-2
  g.addEdge(&data[1], &data[2], 3);  // 2-3
  g.addEdge(&data[2], &data[3], 1);  // 3-4
  g.addEdge(&data[3], &data[0], 5);  // 4-1
  g.addEdge(&data[3], &data[4], 2);  // 4-5
  g.addEdge(&data[4], &data[5], 2);  // 5-6
  g.addEdge(&data[5], &data[2], 4);  // 6-3

  auto cycle = gt::findMinistCircle(&g, gt::MINIST_DIJSKSTRA);
  assert(!cycle.empty());
  assert(isSimpleCycle<int>(cycle));
  assert(totalWeight<int>(cycle) == 9);
}

// ========== Minimum Circle Tests (Floyd-Warshall-based) ==========

void test_minist_circle_floyd_triangle() {
  std::vector<int> data = {1, 2, 3};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1], 1);
  g.addEdge(&data[1], &data[2], 1);
  g.addEdge(&data[2], &data[0], 1);

  auto cycle = gt::findMinistCircle(&g, gt::MINIST_FLOYD_WARSHALL);
  assert(!cycle.empty());
  assert(isSimpleCycle<int>(cycle));
  assert(totalWeight<int>(cycle) == 3);
  assert(cycle.size() == 3);
}

void test_minist_circle_floyd_square() {
  std::vector<int> data = {1, 2, 3, 4};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1], 1);
  g.addEdge(&data[1], &data[2], 1);
  g.addEdge(&data[2], &data[3], 1);
  g.addEdge(&data[3], &data[0], 1);

  auto cycle = gt::findMinistCircle(&g, gt::MINIST_FLOYD_WARSHALL);
  assert(!cycle.empty());
  assert(isSimpleCycle<int>(cycle));
  assert(totalWeight<int>(cycle) == 4);
}

void test_minist_circle_floyd_weighted() {
  // Same graph as Dijkstra weighted test
  std::vector<int> data = {1, 2, 3, 4, 5, 6};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1], 1);
  g.addEdge(&data[1], &data[3], 1);
  g.addEdge(&data[3], &data[2], 2);
  g.addEdge(&data[2], &data[0], 2);
  g.addEdge(&data[2], &data[4], 1);
  g.addEdge(&data[4], &data[5], 1);
  g.addEdge(&data[5], &data[3], 1);

  auto cycle = gt::findMinistCircle(&g, gt::MINIST_FLOYD_WARSHALL);
  assert(!cycle.empty());
  assert(isSimpleCycle<int>(cycle));
  assert(totalWeight<int>(cycle) == 5);
}

void test_minist_circle_floyd_complex() {
  // Same complex graph as Dijkstra test
  std::vector<int> data = {1, 2, 3, 4, 5, 6};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1], 10);
  g.addEdge(&data[1], &data[2], 3);
  g.addEdge(&data[2], &data[3], 1);
  g.addEdge(&data[3], &data[0], 5);
  g.addEdge(&data[3], &data[4], 2);
  g.addEdge(&data[4], &data[5], 2);
  g.addEdge(&data[5], &data[2], 4);

  auto cycle = gt::findMinistCircle(&g, gt::MINIST_FLOYD_WARSHALL);
  assert(!cycle.empty());
  assert(isSimpleCycle<int>(cycle));
  assert(totalWeight<int>(cycle) == 9);
}

// ========== Edge Cases ==========

void test_minist_circle_null_and_small() {
  // Null graph
  gt::Graph<int> *nullGraph = nullptr;
  assert(gt::findMinistCircle(nullGraph, gt::MINIST_DIJSKSTRA).empty());
  assert(gt::findMinistCircle(nullGraph, gt::MINIST_FLOYD_WARSHALL).empty());

  // Graph with only 2 vertices (no cycle possible)
  std::vector<int> data = {1, 2};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1], 1);
  assert(gt::findMinistCircle(&g, gt::MINIST_DIJSKSTRA).empty());
  assert(gt::findMinistCircle(&g, gt::MINIST_FLOYD_WARSHALL).empty());
}

void test_minist_circle_directed_guard() {
  std::vector<int> data = {1, 2, 3};
  gt::DirectedGraph<int> g;
  g.addEdge(&data[0], &data[1], 1);
  g.addEdge(&data[1], &data[2], 1);
  g.addEdge(&data[2], &data[0], 1);

  // Directed graph should return empty
  assert(gt::findMinistCircle(&g, gt::MINIST_DIJSKSTRA).empty());
  assert(gt::findMinistCircle(&g, gt::MINIST_FLOYD_WARSHALL).empty());
}

void test_minist_circle_consistency() {
  // Both algorithms should find the same minimum cycle weight
  std::vector<int> data = {1, 2, 3, 4, 5};
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1], 4);
  g.addEdge(&data[1], &data[2], 2);
  g.addEdge(&data[2], &data[3], 3);
  g.addEdge(&data[3], &data[4], 1);
  g.addEdge(&data[4], &data[0], 5);
  g.addEdge(&data[0], &data[2], 1); // shortcut

  auto cycleDijkstra = gt::findMinistCircle(&g, gt::MINIST_DIJSKSTRA);
  auto cycleFloyd = gt::findMinistCircle(&g, gt::MINIST_FLOYD_WARSHALL);

  assert(!cycleDijkstra.empty());
  assert(!cycleFloyd.empty());
  assert(isSimpleCycle<int>(cycleDijkstra));
  assert(isSimpleCycle<int>(cycleFloyd));
  // Both should find the same minimum weight
  assert(totalWeight<int>(cycleDijkstra) == totalWeight<int>(cycleFloyd));
}

int main() {
  // Existing circle count tests
  TEST_AND_RUN(test_null_and_small_graph);
  TEST_AND_RUN(test_triangle_only);
  TEST_AND_RUN(test_square_with_diagonal);
  TEST_AND_RUN(test_k4_complete_graph);
  TEST_AND_RUN(test_directed_graph_guard);

  // Minimum circle tests - Dijkstra
  TEST_AND_RUN(test_minist_circle_dijkstra_triangle);
  TEST_AND_RUN(test_minist_circle_dijkstra_square);
  TEST_AND_RUN(test_minist_circle_dijkstra_weighted);
  TEST_AND_RUN(test_minist_circle_dijkstra_complex);

  // Minimum circle tests - Floyd-Warshall
  TEST_AND_RUN(test_minist_circle_floyd_triangle);
  TEST_AND_RUN(test_minist_circle_floyd_square);
  TEST_AND_RUN(test_minist_circle_floyd_weighted);
  TEST_AND_RUN(test_minist_circle_floyd_complex);

  // Edge cases
  TEST_AND_RUN(test_minist_circle_null_and_small);
  TEST_AND_RUN(test_minist_circle_directed_guard);
  TEST_AND_RUN(test_minist_circle_consistency);

  std::cout << "All circle count tests passed!" << std::endl;
  return 0;
}

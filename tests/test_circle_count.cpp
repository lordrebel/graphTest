#include "graph_circle_count.hpp"
#include <cassert>
#include <iostream>
#include <vector>

namespace gt = graphTest;

#define TEST_AND_RUN(test_name)                                                \
  std::cout << "Running " << #test_name << "..." << std::endl;                 \
  test_name();                                                                 \
  std::cout << #test_name << " passed!" << std::endl;

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

int main() {
  TEST_AND_RUN(test_null_and_small_graph);
  TEST_AND_RUN(test_triangle_only);
  TEST_AND_RUN(test_square_with_diagonal);
  TEST_AND_RUN(test_k4_complete_graph);
  TEST_AND_RUN(test_directed_graph_guard);
  std::cout << "All circle count tests passed!" << std::endl;
  return 0;
}

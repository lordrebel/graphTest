#include "graph.hpp"
#include "graph_connected_component.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

using namespace graphTest;

template <typename DataT>
std::size_t countNodeType(const typename BlockForestResult<DataT>::Tree &tree,
                          typename BlockForestResult<DataT>::NodeType type) {
  std::size_t cnt = 0;
  for (const auto &node : tree.nodes) {
    if (node.type == type) {
      ++cnt;
    }
  }
  return cnt;
}

template <typename DataT>
const typename BlockForestResult<DataT>::Tree *
findTreeByRoundVertex(const BlockForestResult<DataT> &forest,
                      typename Graph<DataT>::Pointer vertex) {
  for (const auto &tree : forest.trees) {
    for (const auto &node : tree.nodes) {
      if (node.type == BlockForestResult<DataT>::NodeType::Round &&
          node.vertex == vertex) {
        return &tree;
      }
    }
  }
  return nullptr;
}

void testSingleCycle() {
  UnDirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n2, &n3);
  graph.addEdge(&n3, &n1);

  auto forest = getBlockForest(&graph);
  assert(forest.trees.size() == 1);

  const auto &tree = forest.trees.front();
  assert(countNodeType<int>(tree, BlockForestResult<int>::NodeType::Round) ==
         3);
  assert(countNodeType<int>(tree, BlockForestResult<int>::NodeType::Square) ==
         1);
  assert(tree.edges.size() == 3);
  std::cout << "testSingleCycle passed\n";
}

void testArticulationTwoCycles() {
  UnDirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3, n4 = 4, n5 = 5;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n2, &n3);
  graph.addEdge(&n3, &n1);

  graph.addEdge(&n3, &n4);
  graph.addEdge(&n4, &n5);
  graph.addEdge(&n5, &n3);

  auto forest = getBlockForest(&graph);
  assert(forest.trees.size() == 1);

  const auto &tree = forest.trees.front();
  assert(countNodeType<int>(tree, BlockForestResult<int>::NodeType::Round) ==
         5);
  assert(countNodeType<int>(tree, BlockForestResult<int>::NodeType::Square) ==
         2);
  assert(tree.edges.size() == 6);

  std::size_t articulation_degree = 0;
  for (const auto &edge : tree.edges) {
    const auto &u = tree.nodes[edge.first];
    const auto &v = tree.nodes[edge.second];
    if ((u.type == BlockForestResult<int>::NodeType::Round &&
         u.vertex == &n3) ||
        (v.type == BlockForestResult<int>::NodeType::Round &&
         v.vertex == &n3)) {
      ++articulation_degree;
    }
  }
  assert(articulation_degree == 2);
  std::cout << "testArticulationTwoCycles passed\n";
}

void testMultiConnectedComponents() {
  UnDirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3, n4 = 4, n5 = 5;

  graph.addEdge(&n1, &n2);

  graph.addEdge(&n3, &n4);
  graph.addEdge(&n4, &n5);
  graph.addEdge(&n5, &n3);

  auto forest = getBlockForest(&graph);
  assert(forest.trees.size() == 2);

  const auto *tree_with_n1 = findTreeByRoundVertex<int>(forest, &n1);
  const auto *tree_with_n3 = findTreeByRoundVertex<int>(forest, &n3);
  assert(tree_with_n1 != nullptr);
  assert(tree_with_n3 != nullptr);

  assert(countNodeType<int>(*tree_with_n1,
                            BlockForestResult<int>::NodeType::Round) == 2);
  assert(countNodeType<int>(*tree_with_n1,
                            BlockForestResult<int>::NodeType::Square) == 1);
  assert(tree_with_n1->edges.size() == 2);

  assert(countNodeType<int>(*tree_with_n3,
                            BlockForestResult<int>::NodeType::Round) == 3);
  assert(countNodeType<int>(*tree_with_n3,
                            BlockForestResult<int>::NodeType::Square) == 1);
  assert(tree_with_n3->edges.size() == 3);
  std::cout << "testMultiConnectedComponents passed\n";
}

int main() {
  testSingleCycle();
  testArticulationTwoCycles();
  testMultiConnectedComponents();
  std::cout << "All block forest tests passed.\n";
  return 0;
}

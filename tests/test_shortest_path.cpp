#include "graph_shartest_path.hpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;
namespace gt = graphTest;

#define TEST_AND_RUN(test_name)                                                \
  std::cout << "Running " << #test_name << "..." << std::endl;                 \
  test_name();                                                                 \
  std::cout << #test_name << " passed!" << std::endl;

template <typename T>
int totalWeight(const std::vector<typename gt::Graph<T>::Edge>& edges) {
    int sum = 0;
    for (const auto& e : edges) {
        sum += e.weight_;
    }
    return sum;
}

void test_dijkstra() {
  vector<std::string> data = {"S", "A", "B", "C", "D"};
  // S:0, A:1, B:2, C:3, D:4
  gt::DirectedGraph<std::string> g;
  g.addEdge(&data[0], &data[1], 10);
  g.addEdge(&data[0], &data[3], 5);
  g.addEdge(&data[1], &data[2], 1);
  g.addEdge(&data[1], &data[3], 2);
  g.addEdge(&data[3], &data[1], 3);
  g.addEdge(&data[3], &data[2], 9);
  g.addEdge(&data[3], &data[4], 2);
  g.addEdge(&data[4], &data[0], 7);
  g.addEdge(&data[4], &data[2], 6);

  auto edges = gt::ShortestPathOneSource(&g, &data[0], gt::ShortestPathAlgo::DIJKSTRA);
  // Expected dists: S:0, C:5, A:8, D:7, B:9
  // Edges forming the tree: 
  // S->C (5)
  // C->A (3)
  // C->D (2)
  // A->B (1)
  // Total weight: 11
  assert(edges.size() == 4);
  assert(totalWeight<std::string>(edges) == 11);
}

void test_bellman_ford() {
  vector<std::string> data = {"S", "A", "B", "C", "D"};
  gt::DirectedGraph<std::string> g;
  g.addEdge(&data[0], &data[1], 6);
  g.addEdge(&data[0], &data[3], 7);
  g.addEdge(&data[1], &data[2], 5);
  g.addEdge(&data[1], &data[3], 8);
  g.addEdge(&data[1], &data[4], -4); // Negative edge
  g.addEdge(&data[2], &data[1], -2); // Negative edge
  g.addEdge(&data[3], &data[2], -3); // Negative edge
  g.addEdge(&data[3], &data[4], 9);
  g.addEdge(&data[4], &data[0], 2);
  g.addEdge(&data[4], &data[2], 7);

  auto edges = gt::ShortestPathOneSource(&g, &data[0], gt::ShortestPathAlgo::BELLMAN_FORD);
  // Expected tree path weights can be validated similarly
  assert(edges.size() == 4);
}

void test_bellman_ford_negative_cycle() {
  vector<std::string> data = {"A", "B", "C"};
  gt::DirectedGraph<std::string> g;
  g.addEdge(&data[0], &data[1], 1);
  g.addEdge(&data[1], &data[2], -1);
  g.addEdge(&data[2], &data[0], -1); // Negative cycle: 1 - 1 - 1 = -1

  auto edges = gt::ShortestPathOneSource(&g, &data[0], gt::ShortestPathAlgo::BELLMAN_FORD);
  assert(edges.empty()); // Should return empty because of negative cycle
}

void test_floyd_warshall() {
  vector<std::string> data = {"1", "2", "3", "4"};
  gt::DirectedGraph<std::string> g;
  g.addEdge(&data[0], &data[2], -2);
  g.addEdge(&data[1], &data[0], 4);
  g.addEdge(&data[1], &data[2], 3);
  g.addEdge(&data[2], &data[3], 2);
  g.addEdge(&data[3], &data[1], -1);

  auto all_pairs = gt::ShortestPathAllPairs(&g, gt::ShortestPathAlgo::FLOYD_WARSHALL);
  // Check from 2 to 3 distance: 2->1 (4), 1->3 (-2) -> Total distance 2
  // But shortest is: 2->1(4), 1->3(-2) => 2.
  auto shortest_tree_from_2 = all_pairs[&data[1]];
  assert(shortest_tree_from_2.size() == 3);
}


void test_k_shortest_path_astar() {
  std::vector<std::string> data = {"A", "B", "C", "D"};
  gt::DirectedGraph<std::string> g;
  g.addEdge(&data[0], &data[1], 1);
  g.addEdge(&data[1], &data[3], 2);
  g.addEdge(&data[0], &data[2], 2);
  g.addEdge(&data[2], &data[3], 2);
  g.addEdge(&data[1], &data[2], 1);

  // Paths A->D:
  // A->B->D (1+2 = 3)
  // A->C->D (2+2 = 4)
  // A->B->C->D (1+1+2 = 4)
  
  auto path1 = gt::KshortestPath(&g, &data[0], &data[3], gt::KShortestPathAlgo::A_STAR, 1);
  auto path2 = gt::KshortestPath(&g, &data[0], &data[3], gt::KShortestPathAlgo::A_STAR, 2);
  auto path3 = gt::KshortestPath(&g, &data[0], &data[3], gt::KShortestPathAlgo::A_STAR, 3);
  assert(totalWeight<std::string>(path1) == 3);
  assert(totalWeight<std::string>(path2) == 4);
  assert(totalWeight<std::string>(path3) == 4);
}


int main() {
  test_dijkstra();
  test_bellman_ford();
  test_bellman_ford_negative_cycle();
  test_floyd_warshall();
  test_k_shortest_path_astar();
  std::cout << "All shortest path tests passed!" << std::endl;
  return 0;
}

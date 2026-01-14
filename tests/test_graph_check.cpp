#include "graph_check.hpp"
#include <cassert>
namespace gt = graphTest;

#define TEST_AND_RUN(test_name)                                                \
  std::cout << "Running " << #test_name << "..." << std::endl;                 \
  test_name();                                                                 \
  std::cout << #test_name << " passed!" << std::endl;

void test_directedGraph_circle() {
  gt::DirectedGraph<std::string> g;
  std::string data[] = {"A", "B", "C", "D", "E"};
  /*
  graph
      A --> B
      B --> C
      C --> D
      D --> E
      C --> A
  */
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[1], &data[2]);
  g.addEdge(&data[2], &data[3]);
  g.addEdge(&data[3], &data[4]);
  g.addEdge(&data[2], &data[0]); // adding a self-loop

  assert(gt::hasCircle(&g) == true); // should return true
  g.removeEdge(&data[2], &data[0]);
  assert(gt::hasCircle(&g) == false); // should return false
}

void test_undirectedGraph_circle() {
  gt::UnDirectedGraph<std::string> g;
  std::string data[] = {"A", "B", "C", "D", "E"};
  /*
  graph
      A --> B
      B --> C
      C --> D
      D --> E
      C --> A
  */
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[1], &data[2]);
  g.addEdge(&data[2], &data[3]);
  g.addEdge(&data[3], &data[4]);
  g.addEdge(&data[2], &data[0]); // adding a self-loop

  assert(gt::hasCircle(&g) == true); // should return true
  g.removeEdge(&data[2], &data[0]);
  assert(gt::hasCircle(&g) == false); // should return false
}
void test_directedGraph_same() {
  gt::DirectedGraph<std::string> g, g2;
  std::string data[] = {"A", "B", "C", "D", "E"};
  /*
  graph
      A --> B
      B --> C
      C --> D
      D --> E
      E --> A
  */
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[1], &data[2]);
  g.addEdge(&data[2], &data[3]);
  g.addEdge(&data[3], &data[4]);
  g.addEdge(&data[4], &data[0]);

  g2.addEdge(&data[0], &data[1]);
  g2.addEdge(&data[1], &data[2]);
  g2.addEdge(&data[2], &data[3]);
  g2.addEdge(&data[3], &data[4]);
  g2.addEdge(&data[4], &data[0]);

  assert(gt::isSame(&g, &g2) == true);

  g2.removeEdge(&data[4], &data[0]);
  assert(gt::isSame(&g, &g2) == false);
}

void testIsBipartiteDirected() {
  gt::DirectedGraph<std::string> g;
  std::string data[] = {"A", "B", "C", "D", "E", "F", "G"};
  /*
  graph
      A --> B
      B --> C
      C --> D
      D --> E
      E --> F
      F --> G
  */
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[1], &data[2]);
  g.addEdge(&data[2], &data[3]);
  g.addEdge(&data[3], &data[4]);
  g.addEdge(&data[4], &data[5]);
  g.addEdge(&data[5], &data[6]);
  assert(gt::isBipartite(&g) == true);
  g.addEdge(&data[6], &data[0]); // adding a cycle
  assert(gt::isBipartite(&g) == false);

}

void testIsBipartiteUndirected() {
  gt::UnDirectedGraph<std::string> g;
  std::string data[] = {"A", "B", "C", "D", "E", "F", "G"};
  /*
  graph
      A --> B
      B --> C
      C --> D
      D --> E
      E --> F
      F --> G
  */
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[1], &data[2]);
  g.addEdge(&data[2], &data[3]);
  g.addEdge(&data[3], &data[4]);
  g.addEdge(&data[4], &data[5]);
  g.addEdge(&data[5], &data[6]);
  assert(gt::isBipartite(&g) == true);
  g.addEdge(&data[6], &data[0]); // adding a cycle
  assert(gt::isBipartite(&g) == false);
}

int main() {
  TEST_AND_RUN(test_directedGraph_circle);
  TEST_AND_RUN(test_undirectedGraph_circle);
  TEST_AND_RUN(test_directedGraph_same);
  TEST_AND_RUN(testIsBipartiteDirected);
  TEST_AND_RUN(testIsBipartiteUndirected);
  std::cout << "All tests passed!" << std::endl;
}
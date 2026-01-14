// testing graph basic functions
#include "basicAlgo.hpp"
#include "graph.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
namespace gt = graphTest;

#define TEST_AND_RUN(test_name)                                                \
  std::cout << "Running " << #test_name << "..." << std::endl;                 \
  test_name();                                                                 \
  std::cout << #test_name << " passed!" << std::endl;
/*

*/
/**
 * @brief Tests basic functionality of a directed graph implementation.
 *
 * This test function validates the core operations of a directed graph
 * including:
 * - Adding vertices and edges
 * - Calculating in-degrees and out-degrees
 * - Retrieving predecessor and successor vertices
 * - Getting incoming, outgoing, and all edges for vertices
 * - Removing edges and vertices
 * - Verifying graph state after modifications
 *
 * The test creates a directed graph with 5 vertices (1->2, 1->3, 2->4, 3->5,
 * 5->4), performs various queries and modifications, and asserts expected
 * results at each step.
 *
 * @note This function has no parameters as it is a self-contained unit test.
 * @note This function does not return a value. Test failures are indicated by
 * assertion failures.
 */
void test_directed_basic() {
  vector<int> data = {1, 2, 3, 4, 5};
  /*
  graph
      1 -> 2
      1 -> 3
      2 -> 4
      3 -> 5
      5 -> 4
  */
  gt::DirectedGraph<int> g;
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[0], &data[2]);
  g.addEdge(&data[1], &data[3]);
  g.addEdge(&data[2], &data[4]);
  g.addEdge(&data[4], &data[3]);

  assert(g.getOutdegrees(&data[0]) == 2);
  assert(g.getOutdegrees(&data[1]) == 1);
  assert(g.getOutdegrees(&data[2]) == 1);
  assert(g.getOutdegrees(&data[3]) == 0);
  assert(g.getOutdegrees(&data[4]) == 1);

  assert(g.getIndegrees(&data[0]) == 0);
  assert(g.getIndegrees(&data[1]) == 1);
  assert(g.getIndegrees(&data[2]) == 1);
  assert(g.getIndegrees(&data[3]) == 2);
  assert(g.getIndegrees(&data[4]) == 1);

  assert(g.getAllVertexs().size() == 5);
  assert(g.getAllEdges().size() == 5);
  assert(g.numVertexs() == 5);
  assert(g.getPrev(&data[0]).size() == 0);
  assert(g.getPrev(&data[1]).size() == 1);
  assert(g.getPrev(&data[2]).size() == 1);
  assert(g.getPrev(&data[3]).size() == 2);
  assert(g.getPrev(&data[4]).size() == 1);
  assert(g.getNext(&data[0]).size() == 2);
  assert(g.getNext(&data[1]).size() == 1);
  assert(g.getNext(&data[2]).size() == 1);
  assert(g.getNext(&data[3]).size() == 0);
  assert(g.getNext(&data[4]).size() == 1);
  std::vector<graphTest::Graph<int>::Edge> in_edges;
  std::vector<graphTest::Graph<int>::Edge> out_edges;
  std::vector<graphTest::Graph<int>::Edge> all_edges;
  g.getEdges(&data[0], all_edges);
  g.getInEdges(&data[0], in_edges);
  g.getOutEdges(&data[0], out_edges);
  assert(in_edges.size() == 0);
  assert(out_edges.size() == 2);
  assert(all_edges.size() == 2);
  g.getEdges(&data[1], all_edges);
  g.getInEdges(&data[1], in_edges);
  g.getOutEdges(&data[1], out_edges);
  assert(in_edges.size() == 1);
  assert(out_edges.size() == 1);
  assert(all_edges.size() == 2);
  g.getEdges(&data[2], all_edges);
  g.getInEdges(&data[2], in_edges);
  g.getOutEdges(&data[2], out_edges);
  assert(in_edges.size() == 1);
  assert(out_edges.size() == 1);
  assert(all_edges.size() == 2);
  g.getEdges(&data[3], all_edges);
  g.getInEdges(&data[3], in_edges);
  g.getOutEdges(&data[3], out_edges);
  assert(in_edges.size() == 2);
  assert(out_edges.size() == 0);
  assert(all_edges.size() == 2);
  g.getEdges(&data[4], all_edges);
  g.getInEdges(&data[4], in_edges);
  g.getOutEdges(&data[4], out_edges);
  assert(in_edges.size() == 1);
  assert(out_edges.size() == 1);
  assert(all_edges.size() == 2);

  g.removeEdge(gt::Graph<int>::Pointer(&data[1]),
               gt::Graph<int>::Pointer(&data[3]));
  g.removeEdge(gt::Graph<int>::Pointer(&data[0]),
               gt::Graph<int>::Pointer(&data[4]));
  assert(g.getIndegrees(&data[3]) == 1);
  assert(g.getOutdegrees(&data[1]) == 0);
  assert(g.getAllEdges().size() == 4);
  /*
   1 -> 2
  1 -> 3
  3 -> 5
  5 -> 4
  */

  // now graph
  g.removeVertex(gt::Graph<int>::Pointer(&data[0]));
  g.removeVertex(gt::Graph<int>::Pointer(&data[4]));
  /*
 graph
     2
     3
     4
 */

  assert(g.getIndegrees(&data[1]) == 0);
  assert(g.getOutdegrees(&data[1]) == 0);
  assert(g.getIndegrees(&data[2]) == 0);
  assert(g.getOutdegrees(&data[2]) == 0);
  assert(g.getIndegrees(&data[3]) == 0);
  assert(g.getOutdegrees(&data[3]) == 0);
  assert(g.getAllEdges().size() == 0);
  assert(g.numVertexs() == 3);
  assert(g.getAllVertexs().size() == 3);
  assert(g.isDirected() == true);
}

/**
 * @brief Tests adjacency map functionality of a directed graph implementation.
 *
 * This test function validates the adjacency map operations of a directed graph
 * including:
 * - Building a directed graph with weighted edges
 * - Retrieving the forward adjacency map (source -> destination -> weight)
 * - Retrieving the reverse adjacency map (destination -> source -> weight)
 * - Verifying correct mapping of vertices and edge weights in both directions
 *
 * The test creates a directed graph with 5 vertices and 5 weighted edges:
 * (1->2 weight:10, 1->3 weight:20, 2->4 weight:30, 3->5 weight:40, 5->4
 * weight:50), then validates that both adjacency maps correctly represent the
 * graph structure.
 *
 * @note This function has no parameters as it is a self-contained unit test.
 * @note This function does not return a value. Test failures are indicated by
 * assertion failures.
 */
void test_directed_Adjacency() {
  vector<int> data = {1, 2, 3, 4, 5};
  /*
  graph
      1 -> 2
      1 -> 3
      2 -> 4
      3 -> 5
      5 -> 4
  */
  gt::DirectedGraph<int> g;
  for (int i = 0; i < data.size(); i++) {
    g.addVertex(&data[i]);
  }
  g.addEdge(&data[0], &data[1], 10);
  g.addEdge(&data[0], &data[2], 20);
  g.addEdge(&data[1], &data[3], 30);
  g.addEdge(&data[2], &data[4], 40);
  g.addEdge(&data[4], &data[3], 50);

  std::unordered_map<gt::Graph<int>::Pointer,
                     std::unordered_map<gt::Graph<int>::Pointer, int>>
      adjacyMap, reverseAdjacyMap;
  g.getAdjacencyMap(adjacyMap);

  assert(adjacyMap.size() == 5);
  assert(adjacyMap[&data[0]][&data[1]] == 10);
  assert(adjacyMap[&data[0]][&data[2]] == 20);
  assert(adjacyMap[&data[1]][&data[3]] == 30);
  assert(adjacyMap[&data[2]][&data[4]] == 40);
  assert(adjacyMap[&data[4]][&data[3]] == 50);

  g.getReverseAdjacencyMap(reverseAdjacyMap);
  assert(reverseAdjacyMap.size() == 5);
  assert(reverseAdjacyMap[&data[1]][&data[0]] == 10);
  assert(reverseAdjacyMap[&data[2]][&data[0]] == 20);
  assert(reverseAdjacyMap[&data[3]][&data[1]] == 30);
  assert(reverseAdjacyMap[&data[4]][&data[2]] == 40);
  assert(reverseAdjacyMap[&data[3]][&data[4]] == 50);
}

void test_undirected_basic() {
  vector<int> data = {1, 2, 3, 4, 5, 6, 7};
  /*
  graph
      1 -- 2
      1 -- 3
      2 -- 4
      3 -- 5
      5 -- 4
      6 -- 7

  */
  gt::UnDirectedGraph<int> g;
  for (int i = 0; i < data.size(); i++) {
    g.addVertex(&data[i]);
  }
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[1], &data[0]);
  g.addEdge(&data[0], &data[2]);
  g.addEdge(&data[1], &data[3]);
  g.addEdge(&data[2], &data[4]);
  g.addEdge(&data[4], &data[3]);
  g.addEdge(&data[5], &data[6]);
  g.addEdge(&data[6], &data[5]);
  assert(g.getOutdegrees(&data[0]) == 2);
  assert(g.getOutdegrees(&data[1]) == 2);
  assert(g.getOutdegrees(&data[2]) == 2);
  assert(g.getOutdegrees(&data[3]) == 2);
  assert(g.getOutdegrees(&data[4]) == 2);
  assert(g.getOutdegrees(&data[5]) == 1);
  assert(g.getOutdegrees(&data[6]) == 1);

  assert(g.getIndegrees(&data[0]) == 2);
  assert(g.getIndegrees(&data[1]) == 2);
  assert(g.getIndegrees(&data[2]) == 2);
  assert(g.getIndegrees(&data[3]) == 2);
  assert(g.getIndegrees(&data[4]) == 2);
  assert(g.getIndegrees(&data[5]) == 1);
  assert(g.getIndegrees(&data[6]) == 1);

  assert(g.getAllVertexs().size() == 7);
  assert(g.getAllEdges().size() == 6);
  assert(g.numVertexs() == 7);
  assert(g.isDirected() == false);

  g.removeEdge(gt::Graph<int>::Pointer(&data[1]),
               gt::Graph<int>::Pointer(&data[3]));
  g.removeEdge(gt::Graph<int>::Pointer(&data[5]),
               gt::Graph<int>::Pointer(&data[6]));
  assert(g.getIndegrees(&data[3]) == 1);
  assert(g.getOutdegrees(&data[1]) == 1);
  assert(g.getIndegrees(&data[6]) == 0);
  assert(g.getOutdegrees(&data[5]) == 0);
  assert(g.getAllEdges().size() == 4);
  /*
      1 -- 2
      1 -- 3
      3 -- 5
      5 -- 4
      6
      7
  */

  g.removeVertex(gt::Graph<int>::Pointer(&data[0]));
  g.removeVertex(gt::Graph<int>::Pointer(&data[6]));

  /*
  graph
      2
      3 -- 5
      5 -- 4
      6
  */
  assert(g.getIndegrees(&data[0]) == -1);
  assert(g.getIndegrees(&data[1]) == 0);
  assert(g.getOutdegrees(&data[1]) == 0);
  assert(g.getIndegrees(&data[2]) == 1);
  assert(g.getOutdegrees(&data[2]) == 1);
  assert(g.getIndegrees(&data[3]) == 1);
  assert(g.getOutdegrees(&data[3]) == 1);
  assert(g.getIndegrees(&data[4]) == 2);
  assert(g.getOutdegrees(&data[4]) == 2);
  assert(g.getIndegrees(&data[5]) == 0);
  assert(g.getOutdegrees(&data[5]) == 0);
  assert(g.getAllEdges().size() == 2);
  assert(g.numVertexs() == 5);
  assert(g.getAllVertexs().size() == 5);
  assert(g.isDirected() == false);
}

void test_undirected_adjacency() {
  vector<int> data = {1, 2, 3, 4, 5};
  /*
  graph
      1 -> 2
      1 -> 3
      2 -> 4
      3 -> 5
      5 -> 4
  */
  gt::UnDirectedGraph<int> g;
  for (int i = 0; i < data.size(); i++) {
    g.addVertex(&data[i]);
  }
  g.addEdge(&data[0], &data[1], 10);
  g.addEdge(&data[0], &data[2], 20);
  g.addEdge(&data[1], &data[3], 30);
  g.addEdge(&data[2], &data[4], 40);
  g.addEdge(&data[4], &data[3], 50);

  std::unordered_map<gt::Graph<int>::Pointer,
                     std::unordered_map<gt::Graph<int>::Pointer, int>>
      adjacyMap, reverseAdjacyMap;
  g.getAdjacencyMap(adjacyMap);

  assert(adjacyMap.size() == 5);
  assert(adjacyMap[&data[0]][&data[1]] == 10);
  assert(adjacyMap[&data[1]][&data[0]] == 10);

  assert(adjacyMap[&data[0]][&data[2]] == 20);
  assert(adjacyMap[&data[2]][&data[0]] == 20);

  assert(adjacyMap[&data[1]][&data[3]] == 30);
  assert(adjacyMap[&data[3]][&data[1]] == 30);

  assert(adjacyMap[&data[2]][&data[4]] == 40);
  assert(adjacyMap[&data[4]][&data[2]] == 40);

  assert(adjacyMap[&data[4]][&data[3]] == 50);
  assert(adjacyMap[&data[3]][&data[4]] == 50);

  g.getReverseAdjacencyMap(reverseAdjacyMap);
  assert(reverseAdjacyMap.size() == 5);
  assert(reverseAdjacyMap[&data[1]][&data[0]] == 10);
  assert(reverseAdjacyMap[&data[0]][&data[1]] == 10);

  assert(reverseAdjacyMap[&data[2]][&data[0]] == 20);
  assert(reverseAdjacyMap[&data[0]][&data[2]] == 20);

  assert(reverseAdjacyMap[&data[3]][&data[1]] == 30);
  assert(reverseAdjacyMap[&data[1]][&data[3]] == 30);

  assert(reverseAdjacyMap[&data[4]][&data[2]] == 40);
  assert(reverseAdjacyMap[&data[2]][&data[4]] == 40);

  assert(reverseAdjacyMap[&data[3]][&data[4]] == 50);
  assert(reverseAdjacyMap[&data[4]][&data[3]] == 50);
}

void test_reverse_directed_graph() {
  // test_reverse_directed_graph
  vector<int> data = {1, 2, 3, 4, 5, 6, 7};
  /*
  graph
      1 --> 2
      1 --> 3
      2 --> 4
      3 --> 5
      5 --> 4
      6 --> 7

  */
  gt::DirectedGraph<int> g;
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[0], &data[2]);
  g.addEdge(&data[1], &data[3]);
  g.addEdge(&data[2], &data[4]);
  g.addEdge(&data[4], &data[3]);
  g.addEdge(&data[5], &data[6]);

  /*
  reverse graph
      2 --> 1
      3 --> 1
      4 --> 2
      5 --> 3
      4 --> 5
      7 --> 6

  */

  auto rev_g = gt::ReversGraph<int>(&g);
  assert(rev_g->getIndegrees(&data[0]) == 2);
  assert(rev_g->getOutdegrees(&data[0]) == 0);
  assert(rev_g->getIndegrees(&data[1]) == 1);
  assert(rev_g->getOutdegrees(&data[1]) == 1);
  assert(rev_g->getIndegrees(&data[2]) == 1);
  assert(rev_g->getOutdegrees(&data[2]) == 1);
  assert(rev_g->getIndegrees(&data[3]) == 0);
  assert(rev_g->getOutdegrees(&data[3]) == 2);
  assert(rev_g->getIndegrees(&data[4]) == 1);
  assert(rev_g->getOutdegrees(&data[4]) == 1);
  assert(rev_g->getIndegrees(&data[5]) == 1);
  assert(rev_g->getOutdegrees(&data[5]) == 0);
  assert(rev_g->getIndegrees(&data[6]) == 0);
  assert(rev_g->getOutdegrees(&data[6]) == 1);

  assert(rev_g->getAllEdges().size() == 6);
  assert(rev_g->numVertexs() == 7);
  assert(rev_g->getAllVertexs().size() == 7);
  assert(rev_g->isDirected() == true);

  assert(rev_g->getPrev(&data[0]).size() == 2);
  assert(rev_g->getPrev(&data[1])[0] == &data[3]);
  assert(rev_g->getPrev(&data[2])[0] == &data[4]);
  assert(rev_g->getPrev(&data[3]).size() == 0);
  assert(rev_g->getPrev(&data[4])[0] == &data[3]);
  assert(rev_g->getPrev(&data[5])[0] == &data[6]);
  assert(rev_g->getPrev(&data[6]).size() == 0);

  assert(rev_g->getNext(&data[0]).size() == 0);
  assert(rev_g->getNext(&data[1])[0] == &data[0]);
  assert(rev_g->getNext(&data[2])[0] == &data[0]);
  assert(rev_g->getNext(&data[3]).size() == 2);
  assert(rev_g->getNext(&data[4])[0] == &data[2]);
  assert(rev_g->getNext(&data[5]).size() == 0);
  assert(rev_g->getNext(&data[6])[0] == &data[5]);
}
void test_reverse_undirected_graph() {
  // test_reverse_undirected_graph
  vector<int> data = {1, 2, 3, 4, 5, 6, 7};
  /*
  graph
      1 -- 2
      1 -- 3
      2 -- 4
      3 -- 5
      5 -- 4
      6 -- 7

  */
  gt::UnDirectedGraph<int> g;
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[0], &data[2]);
  g.addEdge(&data[1], &data[3]);
  g.addEdge(&data[2], &data[4]);
  g.addEdge(&data[4], &data[3]);
  g.addEdge(&data[5], &data[6]);

  auto rev_g = gt::ReversGraph<int>(&g);
  assert(rev_g->getIndegrees(&data[0]) == 2);
  assert(rev_g->getOutdegrees(&data[0]) == 2);
  assert(rev_g->getIndegrees(&data[1]) == 2);
  assert(rev_g->getOutdegrees(&data[1]) == 2);
  assert(rev_g->getIndegrees(&data[2]) == 2);
  assert(rev_g->getOutdegrees(&data[2]) == 2);
  assert(rev_g->getIndegrees(&data[3]) == 2);
  assert(rev_g->getOutdegrees(&data[3]) == 2);
  assert(rev_g->getIndegrees(&data[4]) == 2);
  assert(rev_g->getOutdegrees(&data[4]) == 2);
  assert(rev_g->getIndegrees(&data[5]) == 1);
  assert(rev_g->getOutdegrees(&data[5]) == 1);
  assert(rev_g->getIndegrees(&data[6]) == 1);
  assert(rev_g->getOutdegrees(&data[6]) == 1);

  assert(rev_g->getAllEdges().size() == 6);
  assert(rev_g->numVertexs() == 7);
  assert(rev_g->getAllVertexs().size() == 7);
  assert(rev_g->isDirected() == false);
  assert(rev_g->getPrev(&data[0]).size() == 2);
}

int main() {

  TEST_AND_RUN(test_directed_basic);
  TEST_AND_RUN(test_undirected_basic);
  TEST_AND_RUN(test_directed_Adjacency);
  TEST_AND_RUN(test_undirected_adjacency);
  TEST_AND_RUN(test_reverse_directed_graph);
  TEST_AND_RUN(test_reverse_undirected_graph);
  return 0;
}
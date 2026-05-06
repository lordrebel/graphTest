#include "eulerian_graph.hpp"
#include "graph.hpp"

#include <cassert>
#include <iostream>

using namespace graphTest;

void testUndirectedEulerian() {
  UnDirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n2, &n3);
  graph.addEdge(&n3, &n1);

  assert(isEulerian(&graph) == true);
  assert(isSemiEulerian(&graph) == false);
  std::cout << "testUndirectedEulerian passed\n";
}

void testUndirectedSemiEulerian() {
  UnDirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n2, &n3);

  assert(isEulerian(&graph) == false);
  assert(isSemiEulerian(&graph) == true);
  std::cout << "testUndirectedSemiEulerian passed\n";
}

void testUndirectedNonEulerian() {
  UnDirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3, n4 = 4;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n1, &n3);
  graph.addEdge(&n1, &n4);

  assert(isEulerian(&graph) == false);
  assert(isSemiEulerian(&graph) == false);
  std::cout << "testUndirectedNonEulerian passed\n";
}

void testDirectedEulerian() {
  DirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n2, &n3);
  graph.addEdge(&n3, &n1);

  assert(isEulerian(&graph) == true);
  assert(isSemiEulerian(&graph) == false);
  std::cout << "testDirectedEulerian passed\n";
}

void testDirectedSemiEulerian() {
  DirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n2, &n1);
  graph.addEdge(&n2, &n3);
  graph.addEdge(&n3, &n1);
  graph.addEdge(&n1, &n3);

  assert(isEulerian(&graph) == false);
  assert(isSemiEulerian(&graph) == true);
  std::cout << "testDirectedSemiEulerian passed\n";
}

void testDirectedNonEulerian() {
  DirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n2, &n1);
  graph.addEdge(&n2, &n3);

  assert(isEulerian(&graph) == false);
  assert(isSemiEulerian(&graph) == false);
  std::cout << "testDirectedNonEulerian passed\n";
}

void testZeroDegreeOnlyGraphs() {
  DirectedGraph<int> directed;
  UnDirectedGraph<int> undirected;
  int n1 = 1, n2 = 2;

  directed.addVertex(&n1);
  undirected.addVertex(&n2);

  assert(isEulerian(&directed) == false);
  assert(isSemiEulerian(&directed) == false);
  assert(isEulerian(&undirected) == false);
  assert(isSemiEulerian(&undirected) == false);
  std::cout << "testZeroDegreeOnlyGraphs passed\n";
}

int main() {
  testUndirectedEulerian();
  testUndirectedSemiEulerian();
  testUndirectedNonEulerian();
  testDirectedEulerian();
  testDirectedSemiEulerian();
  testDirectedNonEulerian();
  testZeroDegreeOnlyGraphs();
  std::cout << "All eulerian tests passed.\n";
  return 0;
}

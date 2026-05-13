#include "graph.hpp"
#include "hamilton_graph.hpp"

#include <cassert>
#include <iostream>

using namespace graphTest;

void testUndirectedHamiltonian() {
  UnDirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n2, &n3);
  graph.addEdge(&n3, &n1);

  assert(isHamiltonian(&graph) == true);
  assert(isSemiHamiltonian(&graph) == false);
  std::cout << "testUndirectedHamiltonian passed\n";
}

void testUndirectedSemiHamiltonian() {
  UnDirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3, n4 = 4;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n2, &n3);
  graph.addEdge(&n3, &n4);

  assert(isHamiltonian(&graph) == false);
  assert(isSemiHamiltonian(&graph) == true);
  std::cout << "testUndirectedSemiHamiltonian passed\n";
}

void testUndirectedNonSemiHamiltonian() {
  UnDirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3, n4 = 4;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n1, &n3);
  graph.addEdge(&n1, &n4);

  assert(isHamiltonian(&graph) == false);
  assert(isSemiHamiltonian(&graph) == false);
  std::cout << "testUndirectedNonSemiHamiltonian passed\n";
}

void testDirectedHamiltonian() {
  DirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n2, &n3);
  graph.addEdge(&n3, &n1);

  assert(isHamiltonian(&graph) == true);
  assert(isSemiHamiltonian(&graph) == false);
  std::cout << "testDirectedHamiltonian passed\n";
}

void testDirectedSemiHamiltonian() {
  DirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3, n4 = 4;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n2, &n3);
  graph.addEdge(&n3, &n4);

  assert(isHamiltonian(&graph) == false);
  assert(isSemiHamiltonian(&graph) == true);
  std::cout << "testDirectedSemiHamiltonian passed\n";
}

void testDirectedNonSemiHamiltonian() {
  DirectedGraph<int> graph;
  int n1 = 1, n2 = 2, n3 = 3, n4 = 4;

  graph.addEdge(&n1, &n2);
  graph.addEdge(&n3, &n4);

  assert(isHamiltonian(&graph) == false);
  assert(isSemiHamiltonian(&graph) == false);
  std::cout << "testDirectedNonSemiHamiltonian passed\n";
}

void testZeroDegreeOnlyGraphs() {
  DirectedGraph<int> directed;
  UnDirectedGraph<int> undirected;
  int n1 = 1, n2 = 2;

  directed.addVertex(&n1);
  undirected.addVertex(&n2);

  assert(isHamiltonian(&directed) == false);
  assert(isSemiHamiltonian(&directed) == false);
  assert(isHamiltonian(&undirected) == false);
  assert(isSemiHamiltonian(&undirected) == false);
  std::cout << "testZeroDegreeOnlyGraphs passed\n";
}

int main() {
  testUndirectedHamiltonian();
  testUndirectedSemiHamiltonian();
  testUndirectedNonSemiHamiltonian();
  testDirectedHamiltonian();
  testDirectedSemiHamiltonian();
  testDirectedNonSemiHamiltonian();
  testZeroDegreeOnlyGraphs();
  std::cout << "All hamilton tests passed.\n";
  return 0;
}

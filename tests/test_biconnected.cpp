#include "graph.hpp"
#include "graph_connected_component.hpp"
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace graphTest;

void testBiconnectedVertexComponent() {
    UnDirectedGraph<int> graph; // undirected graph
    
    int n1 = 1, n2 = 2, n3 = 3, n4 = 4, n5 = 5;

    graph.addVertex(&n1);
    graph.addVertex(&n2);
    graph.addVertex(&n3);
    graph.addVertex(&n4);
    graph.addVertex(&n5);

    // v1-v2-v3 triangle
    graph.addEdge(&n1, &n2);
    graph.addEdge(&n2, &n3);
    graph.addEdge(&n3, &n1);

    // Bridge v3-v4
    graph.addEdge(&n3, &n4);

    // v4-v5 edge
    graph.addEdge(&n4, &n5);

    auto vbccs = getBiConnectedVertexCC(&graph);
    
    assert(vbccs.size() == 3);
    std::cout << "testBiconnectedVertexComponent passed\n";
}

void testBiconnectedEdgeComponent() {
    UnDirectedGraph<int> graph; // undirected graph
    
    int n1 = 1, n2 = 2, n3 = 3, n4 = 4, n5 = 5;

    graph.addVertex(&n1);
    graph.addVertex(&n2);
    graph.addVertex(&n3);
    graph.addVertex(&n4);
    graph.addVertex(&n5);

    // v1-v2-v3 triangle
    graph.addEdge(&n1, &n2);
    graph.addEdge(&n2, &n3);
    graph.addEdge(&n3, &n1);

    // Bridge v3-v4
    graph.addEdge(&n3, &n4);

    // v4-v5 edge
    graph.addEdge(&n4, &n5);

    auto ebccs = getBiConnectedEdgeCC(&graph);
    assert(ebccs.size() == 3);
    std::cout << "testBiconnectedEdgeComponent passed\n";
}

int main() {
    testBiconnectedVertexComponent();
    testBiconnectedEdgeComponent();
    std::cout << "All biconnected component tests passed.\n";
    return 0;
}

#include "graph.hpp"
#include "graph_connected_component.hpp"
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace graphTest;

// Helper to normalize SCC result for comparison
template<typename T>
void normalizeSCC(std::vector<std::vector<T*>>& scc) {
    for (auto& comp : scc) {
        std::sort(comp.begin(), comp.end());
    }
    std::sort(scc.begin(), scc.end(), [](const std::vector<T*>& a, const std::vector<T*>& b) {
        if (a.empty()) return false;
        if (b.empty()) return true;
        if (a.size() != b.size()) return a.size() < b.size();
        for (size_t i = 0; i < a.size(); ++i) {
            if (a[i] != b[i]) return a[i] < b[i];
        }
        return false;
    });
}

void test_scc() {
    DirectedGraph<int> graph;
    int data[5] = {0, 1, 2, 3, 4};
    graph.addVertex(&data[0]);
    graph.addVertex(&data[1]);
    graph.addVertex(&data[2]);
    graph.addVertex(&data[3]);
    graph.addVertex(&data[4]);

    // 0 -> 1 -> 2 -> 0 (SCC 1: 0, 1, 2)
    graph.addEdge(&data[0], &data[1]);
    graph.addEdge(&data[1], &data[2]);
    graph.addEdge(&data[2], &data[0]);

    // 0 -> 3 -> 4 (SCC 2: 3), (SCC 3: 4)
    graph.addEdge(&data[0], &data[3]);
    graph.addEdge(&data[3], &data[4]);

    auto sccTarjan = getSCC(&graph, Tarjan);
    auto sccKosaraju = getSCC(&graph, Kosaraju);

    normalizeSCC(sccTarjan);
    normalizeSCC(sccKosaraju);

    assert(sccTarjan.size() == 3);
    assert(sccTarjan == sccKosaraju);

    std::cout << "SCC tests passed successfully!" << std::endl;
}

int main() {
    std::cout << "Running SCC tests..." << std::endl;
    test_scc();
    return 0;
}

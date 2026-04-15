# graphTest

## Introduction

graphTest is a lightweight C++ graph algorithm library.
Core algorithms are implemented as header-only components under `include/`, and
the project provides CMake-based tests under `tests/`.

## Features

Implemented algorithms and checks:

- Topological sort (including reverse postorder style)
- Bipartite graph check
- Minimum spanning tree: Kruskal, Prim
- Directed minimum spanning tree: Zhu-Liu, Tarjan variant
- Shortest path: Bellman-Ford, Johnson, Dijkstra
- K-shortest path: A*
- Strongly connected components: Tarjan, Kosaraju
- Edge-biconnected components (Tarjan)
- Vertex-biconnected components (Tarjan)
- Block forest (round-square tree forest)


## TODO LIST

- [X] test toposort
- [X] add toposort(rpo)
- [X] Bipartite Graph Check
- [X] Minimum Spanning Tree -kruskal
- [X] Minimum Spanning Tree -prim
- [X] Directed Minimum  Spanning Tree -zhuliu
- [X] Directed Minimum  Spanning Tree -tarjan
- [X] test MST/MSDT
- [X] shortest path:bellman-fold/john/Dijkstra
- [X] K-shortest path: A-star
- [X] SCC :Kosaraju method /tarjan method
- [X] Edge BCC: tarjan
- [X] vertex BCC:tarjan
- [X] block forest
- [ ] circle count
- [ ] minist circle
- [ ] 2-SAT

## Project Layout

- `include/`: graph data structures and algorithms
- `src/`: CMake integration for the library
- `tests/`: executable test cases (using `assert`, not GoogleTest)
- `build/`: generated build directory

## Requirements

- C++17 compatible compiler
- CMake 3.16+

## Build And Run Tests

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Run a single test executable:

```bash
ctest --test-dir build -R test_block_forest --output-on-failure
```

## Quick Example

```cpp
#include "graph.hpp"
#include "graph_connected_component.hpp"
#include <iostream>

using namespace graphTest;

int main() {
	UnDirectedGraph<int> g;
	int a = 1, b = 2, c = 3;

	g.addEdge(&a, &b);
	g.addEdge(&b, &c);
	g.addEdge(&c, &a);

	auto vbcc = getBiConnectedVertexCC(&g);
	auto forest = getBlockForest(&g);

	std::cout << "v-BCC count: " << vbcc.size() << "\n";
	std::cout << "block forest tree count: " << forest.trees.size() << "\n";
	return 0;
}
```

## Notes

- `DirectedGraph<T>` and `UnDirectedGraph<T>` use pointer-based vertices (`T*`).
- SCC algorithms are for directed graphs.
- Biconnected components and block forest are for undirected graphs.

## Roadmap

- Improve API documentation for each algorithm header
- Add more edge-case tests for graph decomposition APIs
- Implement remaining TODO algorithms

# graph.hpp API Reference

This document describes the public API in `include/graph.hpp`.

## 1. Overview

`graph.hpp` defines a pointer-based graph container with:

- Abstract base class: `Graph<dataT>`
- Directed implementation: `DirectedGraph<dataT>`
- Undirected implementation: `UnDirectedGraph<dataT>`

Vertices are represented by raw pointers (`dataT*`). Edges are stored internally using adjacency lists.

## 2. Namespace

All APIs are under:

```cpp
namespace graphTest
```

## 3. Core Types

### 3.1 `Graph<dataT>`

Template base class for graph operations.

Type aliases:

- `Dtype`: same as template parameter `dataT`
- `Pointer`: alias of `dataT*`

### 3.2 Nested `Vertex`

Lightweight vertex wrapper used internally.

- Constructor: `Vertex(Pointer data, size_t id)`
- Constructor: `Vertex(Pointer data)`
- `Pointer get_data() const`
- `size_t get_id() const`
- Equality compares only pointer identity (`data_`), not `id_`.

### 3.3 Nested `Edge`

Directed edge record.

- Constructor: `Edge(const Vertex& from, const Vertex& to, int weight)`
- Constructor: `Edge(const Vertex& from, const Vertex& to)`
- Members: `from`, `to`, `weight_`
- Equality compares `from` and `to` only.

### 3.4 `VertexHash`

Hash functor for `Vertex`, based on `std::hash<Pointer>`.

## 4. Public API of `Graph<dataT>`

### 4.1 Graph kind

```cpp
virtual bool isDirected() const = 0;
```

Returns whether the graph is directed.

### 4.2 Vertex operations

```cpp
void addVertex(Pointer data);
void removeVertex(Pointer data);
virtual size_t numVertexs() const;
virtual std::vector<Pointer> getAllVertexs() const;
```

Behavior:

- `addVertex`: inserts the vertex if it does not exist.
- `removeVertex`: removes vertex and all incident edges.
- `numVertexs`: returns vertex count (note function name is `numVertexs`, not `numVertices`).
- `getAllVertexs`: returns all vertex pointers currently in graph.

### 4.3 Edge operations

```cpp
virtual bool addEdge(Pointer src, Pointer dest, int weight = 0);
virtual void removeEdge(Pointer src, Pointer dest);
virtual std::vector<Edge> getAllEdges() const;
```

Behavior:

- `addEdge`: ensures both endpoint vertices exist; inserts or updates edge weight.
- `removeEdge`: removes edge `src -> dest`.
- `getAllEdges`:
  - In base class: collects from `in_map_` (directed-style storage view).
  - In `UnDirectedGraph`: overridden to avoid returning both directions.

### 4.4 Degree and neighborhood

```cpp
virtual int getIndegrees(Pointer data) const;
virtual int getOutdegrees(Pointer data) const;
virtual std::vector<Pointer> getNext(Pointer data);
virtual std::vector<Pointer> getPrev(Pointer data);
```

Behavior:

- For missing vertex, `getIndegrees` / `getOutdegrees` return `-1`.
- `getNext`: outgoing neighbors.
- `getPrev`: incoming neighbors.

### 4.5 Edge query helpers

```cpp
virtual void getEdges(Pointer data, std::vector<Edge>& res) const;
virtual void getInEdges(Pointer data, std::vector<Edge>& res) const;
virtual void getOutEdges(Pointer data, std::vector<Edge>& res) const;
```

Behavior:

- `getInEdges`: all incoming edges of `data`.
- `getOutEdges`: all outgoing edges of `data`.
- `getEdges`:
  - For directed graph: returns in + out edges.
  - For undirected graph: returns in-edges list (which already contains both directions due to insertion strategy).

Note: these methods call `.at(...)` internally and may throw if vertex does not exist.

### 4.6 Adjacency map export

```cpp
void getAdjacencyMap(std::unordered_map<Pointer, std::unordered_map<Pointer, int>>& adjacyMap) const;
void getReverseAdjacencyMap(std::unordered_map<Pointer, std::unordered_map<Pointer, int>>& adjacyMap) const;
```

Behavior:

- `getAdjacencyMap`: exports `u -> {v: weight}` from outgoing edges.
- `getReverseAdjacencyMap`: exports reverse adjacency from incoming edges.
- Both methods clear output map before filling.

### 4.7 Destructor

```cpp
virtual ~Graph() = default;
```

Polymorphic destruction is supported.

## 5. Derived Classes

### 5.1 `DirectedGraph<dataT>`

```cpp
template <typename dataT> class DirectedGraph : public Graph<dataT>
```

- `isDirected()` returns `true`.
- Uses base `addEdge` behavior (`src -> dest` only).

### 5.2 `UnDirectedGraph<dataT>`

```cpp
template <typename dataT> class UnDirectedGraph : public Graph<dataT>
```

Overrides:

- `isDirected()` returns `false`.
- `addEdge(src, dest, w)` inserts both `src -> dest` and `dest -> src`.
- `removeEdge(src, dest)` removes both directions.
- `getAllEdges()` is deduplicated to represent each undirected relation once.

## 6. Usage Example

```cpp
#include "graph.hpp"
#include <iostream>

using namespace graphTest;

int main() {
  UnDirectedGraph<int> g;
  int a = 1, b = 2, c = 3;

  g.addEdge(&a, &b, 4);
  g.addEdge(&b, &c, 5);

  std::cout << "vertex count: " << g.numVertexs() << "\n";
  std::cout << "a outdegree: " << g.getOutdegrees(&a) << "\n";

  return 0;
}
```

## 7. Notes and Caveats

- Vertex identity is pointer identity (`dataT*`), not value equality.
- This container does not own vertex memory. The caller must keep pointed objects alive.
- Method names are intentionally kept as implemented (`numVertexs`, `getAllVertexs`).
- Some APIs throw on unknown vertices (`getInEdges/getOutEdges/getEdges`), while others return sentinel values (`-1`).

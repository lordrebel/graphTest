
#include "graph_mst.hpp"
#include <cassert>
#include <iostream>
using namespace std;
namespace gt = graphTest;

#define TEST_AND_RUN(test_name)                                                \
  std::cout << "Running " << #test_name << "..." << std::endl;                 \
  test_name();                                                                 \
  std::cout << #test_name << " passed!" << std::endl;

void test_mst_kruskal() {
  vector<std::string> data = {"A", "B", "C", "D", "E", "F", "G"};
  /*
  graph
      A --2-- B
      A --3-- C
      B --1-- C
      B --4-- D
      C --5-- D
      C --6-- E
      D --7-- E
      F --8-- G
  */
  gt::DirectedGraph<std::string> g;
  g.addEdge(&data[0], &data[1], 2);
  g.addEdge(&data[0], &data[2], 3);
  g.addEdge(&data[1], &data[2], 1);
  g.addEdge(&data[1], &data[3], 4);
  g.addEdge(&data[2], &data[3], 5);
  g.addEdge(&data[2], &data[4], 6);
  g.addEdge(&data[3], &data[4], 7);
  g.addEdge(&data[5], &data[6], 8);
  auto mst_graphs = gt::getMSTs<std::string>(&g, gt::KRUSKAL);
  assert(mst_graphs.size() == 0); // directed graph cannot have mst
  gt::UnDirectedGraph<std::string> ug;
  ug.addEdge(&data[0], &data[1], 2);
  ug.addEdge(&data[0], &data[2], 3);
  ug.addEdge(&data[1], &data[2], 1);
  ug.addEdge(&data[1], &data[3], 4);
  ug.addEdge(&data[2], &data[3], 5);
  ug.addEdge(&data[2], &data[4], 6);
  ug.addEdge(&data[3], &data[4], 7);
  ug.addEdge(&data[5], &data[6], 8);
  auto undirected_mst_graphs = gt::getMSTs<std::string>(&ug, gt::KRUSKAL);
  assert(undirected_mst_graphs.size() == 2); // two connected components
  // check first mst
  auto &mst1 = undirected_mst_graphs[0];
  assert(mst1->numVertexs() == 5);
  auto edges1 = mst1->getAllEdges();
  int total_weight1 = 0;
  for (const auto &e : edges1) {
    total_weight1 += e.weight_;
  }
  assert(total_weight1 == 13); // total weight of first MST
  // check second mst
  auto &mst2 = undirected_mst_graphs[1];
  assert(mst2->numVertexs() == 2);
  auto edges2 = mst2->getAllEdges();
  int total_weight2 = 0;
  for (const auto &e : edges2) {
    total_weight2 += e.weight_;
  }
  assert(total_weight2 == 8); // total weight of second MST
}

void test_mst_prim() {
  vector<std::string> data = {"A", "B", "C", "D", "E", "F", "G"};
  /*
  graph
      A --2-- B
      A --3-- C
      B --1-- C
      B --4-- D
      C --5-- D
      C --6-- E
      D --7-- E
      F --8-- G
  */
  gt::DirectedGraph<std::string> g;
  g.addEdge(&data[0], &data[1], 2);
  g.addEdge(&data[0], &data[2], 3);
  g.addEdge(&data[1], &data[2], 1);
  g.addEdge(&data[1], &data[3], 4);
  g.addEdge(&data[2], &data[3], 5);
  g.addEdge(&data[2], &data[4], 6);
  g.addEdge(&data[3], &data[4], 7);
  g.addEdge(&data[5], &data[6], 8);
  auto mst_graphs = gt::getMSTs<std::string>(&g, gt::PRIM);
  assert(mst_graphs.size() == 0); // directed graph cannot have mst
  gt::UnDirectedGraph<std::string> ug;
  ug.addEdge(&data[0], &data[1], 2);
  ug.addEdge(&data[0], &data[2], 3);
  ug.addEdge(&data[1], &data[2], 1);
  ug.addEdge(&data[1], &data[3], 4);
  ug.addEdge(&data[2], &data[3], 5);
  ug.addEdge(&data[2], &data[4], 6);
  ug.addEdge(&data[3], &data[4], 7);
  ug.addEdge(&data[5], &data[6], 8);
  auto undirected_mst_graphs = gt::getMSTs<std::string>(&ug, gt::PRIM);
  assert(undirected_mst_graphs.size() == 2); // two connected components
  // check first mst
  auto &mst1 = undirected_mst_graphs[0];
  assert(mst1->numVertexs() == 5);
  auto edges1 = mst1->getAllEdges();
  int total_weight1 = 0;
  for (const auto &e : edges1) {
    total_weight1 += e.weight_;
  }
  assert(total_weight1 == 13); // total weight of first MST
  // check second mst
  auto &mst2 = undirected_mst_graphs[1];
  assert(mst2->numVertexs() == 2);
  auto edges2 = mst2->getAllEdges();
  int total_weight2 = 0;
  for (const auto &e : edges2) {
    total_weight2 += e.weight_;
  }
  assert(total_weight2 == 8); // total weight of second MST
}

void test_dmst_zhuliu() {
  vector<std::string> data = {"A", "B", "C", "D", "E", "F", "G"};
  gt::UnDirectedGraph<std::string> ug;
  ug.addEdge(&data[0], &data[1], 2);
  ug.addEdge(&data[0], &data[2], 3);
  ug.addEdge(&data[1], &data[2], 1);
  ug.addEdge(&data[1], &data[3], 4);
  ug.addEdge(&data[2], &data[3], 5);
  ug.addEdge(&data[2], &data[4], 6);
  ug.addEdge(&data[3], &data[4], 7);
  ug.addEdge(&data[5], &data[6], 8);
  auto undirected_mst_graphs = gt::getDMSTs<std::string>(&ug, gt::ZHULIU);
  assert(undirected_mst_graphs.size() == 0); // dmst for directed graph only

  gt::DirectedGraph<std::string> dg;
  dg.addEdge(&data[0], &data[1], 2);
  dg.addEdge(&data[0], &data[2], 3);
  dg.addEdge(&data[1], &data[2], 1);
  dg.addEdge(&data[1], &data[3], 4);
  dg.addEdge(&data[2], &data[3], 5);
  dg.addEdge(&data[2], &data[4], 6);
  dg.addEdge(&data[3], &data[4], 7);
  dg.addEdge(&data[5], &data[6], 8);
  auto dmst_graphs = gt::getDMSTs<std::string>(&dg, gt::ZHULIU);
  assert(dmst_graphs.size() == 2); // only one dmst for directed graph
  std::sort(dmst_graphs.begin(), dmst_graphs.end(),
            [](const auto &a, const auto &b) {
              return a->numVertexs() > b->numVertexs();
            });

  auto &dmst1 = dmst_graphs[0]; // 现在 0 肯定是那个 5 节点的了
  assert(dmst1->numVertexs() == 5);
  auto edges1 = dmst1->getAllEdges();
  int total_weight1 = 0;
  for (const auto &e : edges1) {
    total_weight1 += e.weight_;
  }
  assert(total_weight1 == 13);
  auto &dmst2 = dmst_graphs[1];
  assert(dmst2->numVertexs() == 2);
  auto edges2 = dmst2->getAllEdges();
  int total_weight2 = 0;
  for (const auto &e : edges2) {
    total_weight2 += e.weight_;
  }
  assert(total_weight2 == 8);
}
int main() {
  TEST_AND_RUN(test_mst_kruskal);
  TEST_AND_RUN(test_mst_prim);
  TEST_AND_RUN(test_dmst_zhuliu);
  return 0;
}
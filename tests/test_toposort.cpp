#include "graph.hpp"
#include "toposort.hpp"
#include <cassert>
#include <vector>
namespace gt = graphTest;

#define TEST_AND_RUN(test_name)                                                \
  std::cout << "Running " << #test_name << "..." << std::endl;                 \
  test_name();                                                                 \
  std::cout << #test_name << " passed!" << std::endl;

void test_toposortKahn() {
  std::vector<std::string> data = {"A", "B", "C", "D", "E"};
  gt::UnDirectedGraph<std::string> ug;
  for (size_t i = 0; i < 5; ++i) {
    ug.addEdge(&data[i], &data[(i + 1) % 5]);
  }
  std::vector<std::string *> result =
      gt::TopologicalSort(&ug, false, gt::TopoSortAlgo::Kahn);
  assert(result.empty());

  gt::DirectedGraph<std::string> dg;
  /*
  A --> B
  A --> E
  B --> C
  D --> C
  D --> E
  */
  dg.addEdge(&data[0], &data[1]);
  dg.addEdge(&data[0], &data[4]);
  dg.addEdge(&data[1], &data[2]);
  dg.addEdge(&data[3], &data[2]);
  dg.addEdge(&data[3], &data[4]);
  result = gt::TopologicalSort(&dg, false, gt::TopoSortAlgo::Kahn);
  assert(result.size() == 5);
  std::vector<std::string *> correct = {&data[3], &data[0], &data[1], &data[4],
                                        &data[2]};
  assert(result == correct);

  result = gt::TopologicalSort(&dg, true);
  assert(result.size() == 5);

  std::vector<std::string *> reverse_correct = {&data[2], &data[4], &data[1],
                                                &data[3], &data[0]};
  assert(result == reverse_correct);
}

void test_toposortRPO() {
  std::vector<std::string> data = {"A", "B", "C", "D", "E"};
  gt::UnDirectedGraph<std::string> ug;
  for (size_t i = 0; i < 5; ++i) {
    ug.addEdge(&data[i], &data[(i + 1) % 5]);
  }
  std::vector<std::string *> result =
      gt::TopologicalSort(&ug, false, gt::TopoSortAlgo::RPO);
  assert(result.empty());

  gt::DirectedGraph<std::string> dg;
  /*
  A --> B
  A --> E
  B --> C
  D --> C
  D --> E
  */
  dg.addEdge(&data[0], &data[1]);
  dg.addEdge(&data[0], &data[4]);
  dg.addEdge(&data[1], &data[2]);
  dg.addEdge(&data[3], &data[2]);
  dg.addEdge(&data[3], &data[4]);
  result = gt::TopologicalSort(&dg, false, gt::TopoSortAlgo::RPO);
  assert(result.size() == 5);
  std::vector<std::string *> correct = {&data[3], &data[0], &data[4], &data[1],
                                        &data[2]};

  assert(result == correct);

  result = gt::TopologicalSort(&dg, true, gt::TopoSortAlgo::RPO);
  assert(result.size() == 5);

  std::vector<std::string *> reverse_correct = {&data[2], &data[4], &data[3],&data[1],
                                                &data[0]};
  for (auto &item : result)
    std::cout << *item << " ";
  std::cout << std::endl;
  assert(result == reverse_correct);
}

int main() {
  TEST_AND_RUN(test_toposortKahn);
  TEST_AND_RUN(test_toposortRPO);
  std::cout << "All tests passed!" << std::endl;
  return 0;
}
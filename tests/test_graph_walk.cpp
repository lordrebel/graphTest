#include "graph.hpp"
#include "graph_walk.hpp"
#include <cassert>
#include <functional>
#include <string>
#define TEST_AND_RUN(test_name)                                                \
  std::cout << "Running " << #test_name << "..." << std::endl;                 \
  test_name();                                                                 \
  std::cout << #test_name << " passed!" << std::endl;

namespace gt = graphTest;
void test_DirectGraph_DFS_walk() {

  gt::DirectedGraph<std::string> g;
  std::string data[] = {"A", "B", "C", "D", "E", "F", "G"};
  /*
  graph
      A --> B
      A --> C
      B --> D
      C --> E
      E --> D
      F --> G
  */
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[0], &data[2]);
  g.addEdge(&data[1], &data[3]);
  g.addEdge(&data[2], &data[4]);
  g.addEdge(&data[4], &data[3]);
  g.addEdge(&data[5], &data[6]);

  std::vector<std::string> visit_order;
  std::function<void(std::string *)> func = [&visit_order](std::string *data) {
    // std::cout<<"visisted:"<<*data<<std::endl;
    visit_order.push_back(*data);
  };
  gt::Walk(gt::WalkAlgo::DFS_PREORDER, &g, false, func);
  std::vector<std::string> expected_DFS_pre_order = {"A", "B", "D", "C",
                                                     "E", "F", "G"};
  assert(expected_DFS_pre_order == visit_order);

  visit_order.clear();
  gt::Walk(gt::WalkAlgo::DFS_POSTORDER, &g, false, func);
  std::vector<std::string> expected_DFS_post_order = {"D", "B", "E", "C",
                                                      "A", "G", "F"};
  assert(expected_DFS_post_order == visit_order);
}

void test_DirectGraph_BFS_walk() {
  gt::DirectedGraph<std::string> g;
  std::string data[] = {"A", "B", "C", "D", "E", "F", "G"};
  /*
  graph
      A --> B
      A --> C
      B --> D
      C --> E
      E --> D
      F --> G
  */
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[0], &data[2]);
  g.addEdge(&data[1], &data[3]);
  g.addEdge(&data[2], &data[4]);
  g.addEdge(&data[4], &data[3]);
  g.addEdge(&data[5], &data[6]);

  std::vector<std::string> visit_order;
  std::function<void(std::string *)> func = [&visit_order](std::string *data) {
    std::cout << "------->visisted:" << *data << std::endl;
    visit_order.push_back(*data);
  };
  gt::Walk(gt::WalkAlgo::BFS_PREORDER, &g, false, func);
  std::vector<std::string> expected_BFS_pre_order = {"F", "A", "G", "B",
                                                     "C", "D", "E"};
  assert(expected_BFS_pre_order == visit_order);

  visit_order.clear();
  gt::Walk(gt::WalkAlgo::BFS_POSTORDER, &g, false, func);
  std::vector<std::string> expected_BFS_post_order = {"E", "D", "C", "B",
                                                      "G", "A", "F"};
  assert(expected_BFS_post_order == visit_order);
}

void test_UndirectGraph_DFS_walk() {

  gt::UnDirectedGraph<std::string> g;
  std::string data[] = {"A", "B", "C", "D", "E", "F", "G"};
  /*
  graph
      A -- B
      A -- C
      B -- D
      C -- E
      E -- D
      F -- G
  */
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[0], &data[2]);
  g.addEdge(&data[1], &data[3]);
  g.addEdge(&data[2], &data[4]);
  g.addEdge(&data[4], &data[3]);
  g.addEdge(&data[5], &data[6]);

  std::vector<std::string> visit_order;
  std::function<void(std::string *)> func = [&visit_order](std::string *data) {
    // std::cout<<"------->visisted:"<<*data<<std::endl;
    visit_order.push_back(*data);
  };
  gt::Walk(gt::WalkAlgo::DFS_PREORDER, &g, false, func, {&data[0], &data[5]});
  std::vector<std::string> expected_DFS_pre_order1 = {"A", "B", "D", "E",
                                                      "C", "F", "G"};
  std::vector<std::string> expected_DFS_pre_order2 = {"F", "G", "A", "B",
                                                      "D", "E", "C"};
  assert(expected_DFS_pre_order1 == visit_order ||
         expected_DFS_pre_order2 == visit_order);

  visit_order.clear();
  gt::Walk(gt::WalkAlgo::DFS_POSTORDER, &g, false, func, {&data[0], &data[5]});
  std::vector<std::string> expected_DFS_post_order1 = {"C", "E", "D", "B",
                                                       "A", "G", "F"};
  std::vector<std::string> expected_DFS_post_order2 = {"G", "F", "C", "E",
                                                       "D", "B", "A"};
  assert(expected_DFS_post_order1 == visit_order ||
         expected_DFS_post_order2 == visit_order);
}

void test_UndirectGraph_BFS_walk() {
  // TODO
  gt::UnDirectedGraph<std::string> g;
  std::string data[] = {"A", "B", "C", "D", "E", "F", "G"};
  /*
  graph
      A --> B
      A --> C
      B --> D
      C --> E
      E --> D
      F --> G
  */
  g.addEdge(&data[0], &data[1]);
  g.addEdge(&data[0], &data[2]);
  g.addEdge(&data[1], &data[3]);
  g.addEdge(&data[2], &data[4]);
  g.addEdge(&data[4], &data[3]);
  g.addEdge(&data[5], &data[6]);

  std::vector<std::string> visit_order;
  std::function<void(std::string *)> func = [&visit_order](std::string *data) {
    std::cout << "------->visisted:" << *data << std::endl;
    visit_order.push_back(*data);
  };
  gt::Walk(gt::WalkAlgo::BFS_PREORDER, &g, false, func, {&data[0], &data[5]});
  std::vector<std::string> expected_BFS_pre_order = {"A", "F", "B", "C",
                                                     "G", "D", "E"};
  assert(expected_BFS_pre_order == visit_order);

  visit_order.clear();
  gt::Walk(gt::WalkAlgo::BFS_POSTORDER, &g, false, func, {&data[0], &data[5]});
  std::vector<std::string> expected_BFS_post_order = {"E", "D", "G", "C",
                                                      "B", "F", "A"};
  assert(expected_BFS_post_order == visit_order);
}
int main() {
  TEST_AND_RUN(test_DirectGraph_DFS_walk);
  TEST_AND_RUN(test_DirectGraph_BFS_walk);
  TEST_AND_RUN(test_UndirectGraph_DFS_walk);
  TEST_AND_RUN(test_UndirectGraph_BFS_walk);
  std::cout << "All tests passed!" << std::endl;
}
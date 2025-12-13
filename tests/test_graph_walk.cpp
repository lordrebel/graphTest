#include "graph.hpp"
#include "graph_walk.hpp"
#include <cassert>
#include <functional>
#include <string>
#define TEST_AND_RUN(test_name) \
    std::cout << "Running " << #test_name << "..." << std::endl; \
    test_name(); \
    std::cout << #test_name << " passed!" << std::endl;

namespace gt=graphTest;
void test_DirectGraph_DFS_walk(){

    gt::DirectedGraph<std::string> g;
    std::string data[]={"A","B","C","D","E","F","G"};
    /*
    graph
        A --> B
        A --> C
        B --> D
        C --> E
        E --> D
        F --> G
    */
    for(int i=0;i<7;i++){
        g.addVertex(&data[i]);
    }
    g.addEdge(&data[0],&data[1]);
    g.addEdge(&data[0],&data[2]);
    g.addEdge(&data[1],&data[3]);
    g.addEdge(&data[2],&data[4]);
    g.addEdge(&data[4],&data[3]);
    g.addEdge(&data[5],&data[6]);

    std::vector<std::string > visit_order;
    std::function<void (std::string *)> func=[&visit_order](std::string * data){
        std::cout<<"visisted:"<<*data<<std::endl;
        visit_order.push_back(*data);
    };
    gt::Walk(gt::WalkAlgo::DFS_PREORDER, &g,false,func);
    std::vector<std::string> expected_DFS_pre_order={"A","B","D","C","E","F","G"};
    assert(expected_DFS_pre_order == visit_order);
    
    visit_order.clear();
    gt::Walk(gt::WalkAlgo::DFS_POSTORDER, &g,false,func);
    //TODO



}

void test_DirectGraph_BFS_walk(){

}

void test_UndirectGraph_DFS_walk(){

    

}

void test_UndirectGraph_BFS_walk(){

}
int main(){
    TEST_AND_RUN(test_DirectGraph_DFS_walk);

}
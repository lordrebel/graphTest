#pragma once
#include "graph.hpp"
#include <functional>
#include <memory>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include<vector>
namespace graphTest {

template <typename T>
std::vector<T*> DFS(const std::vector<T*>& starts,
                   std::function<std::vector<T*>(T*)>& getNexts,
                   const std::function<void(T*)>& preVisit = nullptr,
                   const std::function<void(T*)>& postVisit = nullptr) {
    if (starts.empty()) return {};
    
    std::vector<T*> order;
    std::unordered_set<T*> visited;
    std::unordered_set<T*> finished;  // 记录哪些节点已经完全处理完了
    std::stack<T*> stk;
    
    for (auto start : starts) {
        if (start != nullptr && visited.count(start) == 0) {
            stk.push(start);
        }
    }
    
    while (!stk.empty()) {
        T* curr = stk.top();
        
        // 如果这个节点还没访问过
        if (visited.count(curr) == 0) {
            visited.insert(curr);
            order.push_back(curr);
            
            if (preVisit) {
                preVisit(curr);
            }
            
            // 把子节点压栈
            std::vector<T*> nexts = getNexts(curr);
            for (auto it = nexts.rbegin(); it != nexts.rend(); ++it) {
                if (*it != nullptr && visited.count(*it) == 0) {
                    stk.push(*it);
                }
            }
        } 
        // 如果访问过了，检查子节点是否都处理完了
        else if (finished.count(curr) == 0) {
            std::vector<T*> nexts = getNexts(curr);
            bool all_finished = true;
            
            for (auto next : nexts) {
                if (next != nullptr && visited.count(next) > 0 && finished.count(next) == 0) {
                    all_finished = false;
                    break;
                }
            }
            
            // 所有子节点都处理完了
            if (all_finished) {
                stk.pop();
                finished.insert(curr);
                
                if (postVisit) {
                    postVisit(curr);
                }
            }
        } else {
            stk.pop();  // 已经完全处理过了
        }
    }
    
    return order;
}

template <typename T> 
std::vector<T *> BFS(const std::vector<T *>& starts,
                    std::function<std::vector<T*>(T *)>& getNexts,
                    const std::function<void(T *)>& preVisit=nullptr,
                    const std::function<void(T *)>& postVisit=nullptr){
    if (starts.empty()) return {};
    std::vector<T *> order;
    std::unordered_set<T *> visited;
    std::queue<T *>q;
    for(auto start:starts){
            if(start!=nullptr && visited.count(start)==0){
                q.push(start);
                visited.insert(start);
            }
        }

    while(!q.empty()){
        T * curr = q.front();
        q.pop();
        order.emplace_back(curr);
        if(preVisit){
            preVisit(curr);
        }
        std::vector<T*> nexts = getNexts(curr);
        for (auto & next : nexts) {
            if (next != nullptr && visited.count(next) == 0) {
                q.push(next);
                visited.insert(next); 
            }
        }
        if(postVisit){
            postVisit(curr);
        }
    }
    return order;
}
template<typename DataT> 
class UnionFind{
public:
UnionFind()=default;
void add(DataT * data){
    if(id_map_.find(data)!=id_map_.end()){
        return;
    }
    int id=parent_.size();
    id_map_[data]=id;
    reverse_id_map_[id]=data;
    parent_.push_back(id);
}

void merge(DataT * data1, DataT * data2){
    if(data1==nullptr || data2==nullptr){
        return;
    }
    if(id_map_.find(data1)==id_map_.end() || id_map_.find(data2)==id_map_.end()){
        return;
    }
    int id1=id_map_[data1];
    int id2=id_map_[data2];
    int parent1=findParent(id1);
    int parent2=findParent(id2);
    if(parent1!=parent2){
        parent_[parent1]=parent2;
    }
}

DataT * findPArent(DataT * data){
    if(data==nullptr){
        return nullptr;
    }
    if(id_map_.find(data)==id_map_.end()){
        return nullptr;
    }
    int id=id_map_[data];
    int parent_id=findParent(id);
    if(reverse_id_map_.find(parent_id)!=reverse_id_map_.end()){
        return reverse_id_map_[parent_id];
    }
    return nullptr;
}

std::unordered_map<DataT *, std::vector<DataT *>> getGroups(){
    std::unordered_map<DataT *, std::vector<DataT *>> groups;
    for(const auto& pair:id_map_){
        DataT * data=pair.first;
        DataT * parent_data=findPArent(data);
        if(parent_data!=nullptr){
            groups[parent_data].push_back(data);
        }
    }
    return groups;
}

private:
int findParent(int id){
    if(parent_[id]!=id){
        parent_[id]=findParent(parent_[id]);
    }
    return parent_[id];
}
std::unordered_map<DataT*, int> id_map_;
std::unordered_map<int, DataT*> reverse_id_map_;
std::vector<int> parent_;
};

template<typename T> 
std::unique_ptr<Graph<T>> CloneGraph(Graph<T> * g){
    if(g==nullptr){
        return nullptr;
    }
    std::unique_ptr<Graph<T>> new_g;
    if(g->isDirected()){
        new_g=std::make_unique<DirectedGraph<T>>();
    }else{
        new_g=std::make_unique<UnDirectedGraph<T>>();
    }
    for(const auto& vertex:g->getAllVertexs()){
        new_g->addVertex(vertex);
    }
    for(const auto& edge:g->getAllEdges()){
        new_g->addEdge(edge.from.get_data(), edge.to.get_data(), edge.weight_);
    }
    return new_g;

}

template<typename T> 
std::unique_ptr<Graph<T>> ReversGraph(Graph<T> * g){
    if(g==nullptr){
        return nullptr;
    }
    std::unique_ptr<Graph<T>> new_g;
    if(g->isDirected()){
        new_g=std::make_unique<DirectedGraph<T>>();
    } else{
        new_g=std::make_unique<UnDirectedGraph<T>>();
    }
    for(const auto& vertex:g->getAllVertexs()){
        new_g->addVertex(vertex);
    }
    for(const auto& edge:g->getAllEdges()){
        new_g->addEdge(edge.to.get_data(), edge.from.get_data(), edge.weight_);
    }
    return new_g;
}



}

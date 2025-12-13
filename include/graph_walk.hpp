#pragma  once 
#include<functional>
#include <iostream>
#include"graph.hpp"
#include"basicAlgo.hpp"
namespace graphTest {
    enum WalkAlgo {
        BFS_PREORDER,
        BFS_POSTORDER,
        DFS_PREORDER,
        DFS_POSTORDER,
    };

    template<typename DataT>
    void Walk(WalkAlgo algo,Graph<DataT> * g,bool reverse,const std::function<void(DataT *)>& walker,std::vector<DataT *>starts={}) {
        if(starts.empty() && !g->isDirected()){
           std::cerr<<"walk on undirected graph must provide start nodes"<<std::endl;
           return;
        }else if(starts.empty()){
            if(!reverse){
                //collect all nodes with zero indegree as start nodes
                for(const auto& data:g->getAllVertexs()){
                    if(g->getIndegrees(data)==0){
                        starts.push_back(data);
                    }
                }
            }else{
                //collect all nodes with zero outdegree as start nodes
                for(const auto& data:g->getAllVertexs()){
                    if(g->getOutdegrees(data)==0){
                        starts.push_back(data);
                    }
                }
            }
        }
        switch (algo) {
            case BFS_PREORDER: {
                std::function<std::vector<DataT *>(DataT *)> getNexts = [g, reverse](DataT * data) {
                     std::vector<DataT *> nexts;
                    if(reverse){
                        nexts=g->getPrev(data);
                    }else{
                        nexts=g->getNext(data);
                    }
                    return nexts;
                };
                BFS<DataT>(starts, getNexts, walker, nullptr);
                break;
            }
            case BFS_POSTORDER: {
                std::function<std::vector<DataT *>(DataT *)> getNexts = [g, reverse](DataT * data) {
                     std::vector<DataT *> nexts;
                    if(reverse){
                        nexts=g->getPrev(data);
                    }else{
                        nexts=g->getNext(data);
                    }
                    return nexts;
                };
                BFS<DataT>(starts, getNexts, nullptr, walker);
                break;
            }
            case DFS_PREORDER: {
                std::function<std::vector<DataT *>(DataT *)> getNexts = [g, reverse](DataT * data) {
                     std::vector<DataT *> nexts;
                    if(reverse){
                        nexts=g->getPrev(data);
                    }else{
                        nexts=g->getNext(data);
                    }
                    return nexts;
                };
                DFS<DataT>(starts, getNexts, walker, nullptr);
                break;
            }
            case DFS_POSTORDER: {
                std::function<std::vector<DataT *>(DataT *)> getNexts = [g, reverse](DataT * data) {
                     std::vector<DataT *> nexts;
                    if(reverse){
                        nexts=g->getPrev(data);
                    }else{
                        nexts=g->getNext(data);
                    }
                    return nexts;
                };
                DFS<DataT>(starts, getNexts, nullptr, walker);
                break;
            }
            default:
                std::cerr << "Unknown walk algorithm!" << std::endl;
                break; 
            
        }
    }

}
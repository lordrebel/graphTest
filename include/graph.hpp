#pragma once
#include <cstddef>
#include <functional>
#include<list>
#include <unordered_map>
#include <unordered_set>
#include <vector>
namespace graphTest {

    struct  VertexHash;
    template<typename dataT> 
    class Graph{
        public:
            typedef dataT Dtype; 
            typedef dataT* Pointer ;
            void addVertex(Pointer data){
                auto vertex=Vertex(data,vertex_id_counter_++);
                if(out_map_.find(vertex)==out_map_.end()){
                    out_map_[vertex]=std::list<Edge>{};
                }
                if(in_map_.find(vertex)==in_map_.end()){
                    in_map_[vertex]=std::list<Edge>{};
                }
            }
            virtual bool isDirected()=0;

            void removeVertex(Pointer data){
                auto it=out_map_.find(Vertex(data));
                if(it!=out_map_.end()){
                    out_map_.erase(it);
                }
                it=in_map_.find(Vertex(data));
                if(it!=in_map_.end()){
                    in_map_.erase(it);
                }
                //remove all edges related to this vertex
                for(auto& pair:out_map_){
                    pair.second.remove_if([data](const Edge& e){ return e.to==data;});
                }
                for(auto& pair:in_map_){
                    pair.second.remove_if([data](const Edge& e){ return e.from==data;});
                }
            }

            void getAdjacencyMap(std::unordered_map<Pointer,std::unordered_map<Pointer, int>> & adjacyMap){
                adjacyMap.clear(); // 先清空输入的矩阵
    
                // 遍历所有顶点，初始化矩阵
                for (const auto& vertex_pair : out_map_) {
                    Pointer src_data = vertex_pair.first.get_data();
                    
                    // 确保每个顶点在矩阵中都有一个条目
                    if (adjacyMap.find(src_data) == adjacyMap.end()) {
                        adjacyMap[src_data] = std::unordered_map<Pointer, int>();
                    }
                    
                    // 遍历该顶点的所有出边
                    for (const auto& edge : vertex_pair.second) {
                        Pointer dest_data = edge.to.get_data();
                        adjacyMap[src_data][dest_data] = edge.weight_;
                    }
                }
                
                // 确保in_map_中的顶点也被包含在矩阵中
                for (const auto& vertex_pair : in_map_) {
                    Pointer dest_data = vertex_pair.first.get_data();
                    if (adjacyMap.find(dest_data) == adjacyMap.end()) {
                        adjacyMap[dest_data] = std::unordered_map<Pointer, int>();
                    }
                }
            }

            void getReverseAdjacencyMap(std::unordered_map<Pointer,std::unordered_map<Pointer, int>> & adjacyMap){
                adjacyMap.clear(); // 先清空输入的邻接表
                
                // 遍历所有顶点，初始化反向邻接表
                for (const auto& vertex_pair : in_map_) {
                    Pointer dest_data = vertex_pair.first.get_data();
                    
                    // 确保每个顶点在反向邻接表中都有一个条目
                    if (adjacyMap.find(dest_data) == adjacyMap.end()) {
                        adjacyMap[dest_data] = std::unordered_map<Pointer, int>();
                    }
                    
                    // 遍历该顶点的所有入边
                    for (const auto& edge : vertex_pair.second) {
                        Pointer src_data = edge.from.get_data();
                        adjacyMap[dest_data][src_data] = edge.weight_;
                    }
                }
                
                // 确保out_map_中的顶点也被包含在反向邻接表中
                for (const auto& vertex_pair : out_map_) {
                    Pointer src_data = vertex_pair.first.get_data();
                    if (adjacyMap.find(src_data) == adjacyMap.end()) {
                        adjacyMap[src_data] = std::unordered_map<Pointer, int>();
                    }
                }
            }

            //edge：src -->dest
            virtual bool addEdge(Pointer src, Pointer dest, int weight=0){
                    Edge cur_edge{Vertex(src),Vertex(dest),weight};

                    auto in_map_iter=in_map_.find(Vertex(dest));
                    auto out_map_iter=out_map_.find(Vertex(src));
                    if(in_map_iter!=in_map_.end()){
                        auto edge_iter=std::find(in_map_iter->second.begin(),
                                    in_map_iter->second.end(),cur_edge);
                        if(edge_iter==in_map_iter->second.end()){
                            in_map_iter->second.push_back(cur_edge);
                        }else{
                            //weight exists, update it
                            edge_iter->weight_=weight;
                        }
                    }else{
                        in_map_[Vertex(dest)].push_back(cur_edge);
                    }
                    if(out_map_iter!=out_map_.end()){
                        auto edge_iter=std::find(out_map_iter->second.begin(),
                                    out_map_iter->second.end(),cur_edge);
                        if(edge_iter==out_map_iter->second.end()){
                            out_map_iter->second.push_back(cur_edge);
                        }else{
                            //weight exists, update it
                            edge_iter->weight_=weight;
                        }
                    }else {
                        out_map_[Vertex(src)].push_back(cur_edge);
                    }
                    return true;
            }
            virtual void removeEdge(Pointer src, Pointer dest){
                Edge cur_edge{Vertex(src),Vertex(dest)};

                auto in_map_iter=in_map_.find(Vertex(dest));
                auto out_map_iter=out_map_.find(Vertex(src));
                if(in_map_iter!=in_map_.end()){
                    in_map_iter->second.remove_if([cur_edge](const Edge& e){ return e==cur_edge;});
                }
                if(out_map_iter!=out_map_.end()){
                    out_map_iter->second.remove_if([cur_edge](const Edge& e){ return e==cur_edge;});
                }
                
            }
            virtual int getIndegrees(Pointer data){
                if(in_map_.find(Vertex(data))!=in_map_.end()){
                    return in_map_[Vertex(data)].size();
                }else return -1;

            }
            virtual int getOutdegrees(Pointer data){
                if(out_map_.find(Vertex(data))!=out_map_.end()){
                    return out_map_[Vertex(data)].size();
                }else return -1;
            }
            virtual size_t numVertexs(){
                return in_map_.size();
            }
            virtual std::vector<Pointer> getAllVertexs(){
                std::vector<Pointer> vertexs;
                for(const auto& pair:in_map_){
                    vertexs.push_back(pair.first.get_data());
                }
                return vertexs;
            }
            virtual std::vector<Pointer> getNext(Pointer data){
                std::vector<Pointer> nexts;
                auto it=out_map_.find(Vertex(data));
                if(it!=out_map_.end()){
                    for(const auto& edge:it->second){
                        nexts.push_back(edge.to.get_data());
                    }
                }
                return nexts;

            }
            virtual std::vector<Pointer> getPrev(Pointer data){
                std::vector<Pointer> prevs;
                auto it=in_map_.find(Vertex(data));
                if(it!=in_map_.end()){
                    for(const auto& edge:it->second){
                        prevs.push_back(edge.from.get_data());
                    }
                }
                return prevs;
            }
            class Vertex{
                public:
                    Vertex(Pointer data,size_t id):data_(data),id_(id){}
                    Vertex(Pointer data):data_(data),id_(0){}
                    Pointer get_data() const { return data_; }
                    size_t get_id() const { return id_; }
                    bool operator ==(const Vertex& other) const { return data_== other.data_; }
                    bool operator ==(const Pointer otherData) const { return data_== otherData; }

                private:
                    Pointer data_;
                    size_t id_;
            };
            struct Edge{
                Edge(const Vertex& from, const Vertex&  to, int weight):from(from),to(to),weight_(weight){}
                Edge(const Vertex&  from, const Vertex&  to):from(from),to(to),weight_(0){}
                Vertex  from;
                Vertex  to;
                int weight_;
                bool operator ==(const Edge& other) const { return from== other.from && to== other.to; }
                
            };
            struct VertexHash{
                std::size_t operator()(const Vertex& vertex) const {
                    return std::hash<Pointer>()(vertex.get_data());
                }
            };
            
            virtual std::vector<Edge>  getAllEdges(){
                std::vector<Edge> res;
                for(const auto& pair:in_map_){
                    std::copy(pair.second.begin(),
                              pair.second.end(),
                              std::back_inserter(res));
                }
                return res;

            }

            virtual void getEdges(Pointer data, std::vector<Edge>& res){
                res.clear();
                std::copy(in_map_[Vertex(data)].begin(),
                              in_map_[Vertex(data)].end(),
                              std::back_inserter(res));
                if(isDirected()){
                    std::copy(out_map_[Vertex(data)].begin(),
                              out_map_[Vertex(data)].end(),
                              std::back_inserter(res));    
                }
            }
            virtual void getInEdges(Pointer data, std::vector<Edge>&res){
                res.clear();
                std::copy(in_map_[Vertex(data)].begin(),
                          in_map_[Vertex(data)].end(),
                          std::back_inserter(res));
                return;
            }
            virtual void getOutEdges(Pointer data, std::vector<Edge>&res){
                res.clear();
                std::copy(out_map_[Vertex(data)].begin(),
                          out_map_[Vertex(data)].end(),
                          std::back_inserter(res));
                return;

            }

        virtual ~Graph()=default;
            
        protected:
            std::unordered_map<Vertex,std::list<Edge>,VertexHash> in_map_;
            std::unordered_map<Vertex,std::list<Edge>,VertexHash> out_map_;
            size_t vertex_id_counter_=0;           
    };

    template<typename dataT>
    class DirectedGraph : public Graph<dataT> {
    public:
        // 构造函数
        DirectedGraph():Graph<dataT>() {};
        bool isDirected() override {
            return true;
        }


    };

    template<typename dataT>
    class UnDirectedGraph : public Graph<dataT> {
    public:
        // 构造函数
        UnDirectedGraph():Graph<dataT>() {};
        bool isDirected() override {
            return false;
        }
        bool addEdge(typename Graph<dataT>::Pointer src, typename Graph<dataT>::Pointer dest, int weight=0) override {
            // 添加从src到dest的边
            Graph<dataT>::addEdge(src, dest, weight);
            // 添加从dest到src的边（无向图的另一半）
            Graph<dataT>::addEdge(dest, src, weight);
            return true;
        }

        std::vector<typename Graph<dataT>::Edge>  getAllEdges()override{
            std::vector<typename Graph<dataT>::Edge> res;
            std::unordered_set<typename Graph<dataT>::Pointer> visited;
            for(auto & pair:this->in_map_){
                for(auto& edge:pair.second){
                   if(visited.count(edge.from.get_data()) == 0){
                        res.push_back(edge);
                   }
                }
                visited.insert(pair.first.get_data());
                
            }
            return res;
        }

        void removeEdge(typename Graph<dataT>::Pointer src, typename Graph<dataT>::Pointer dest) override {
            // 移除从src到dest的边
            Graph<dataT>::removeEdge(src, dest);
            // 移除从dest到src的边（无向图的另一半）
            Graph<dataT>::removeEdge(dest, src);
        }

    };

}
#ifndef _QPGRAPH_H_
#define _QPGRAPH_H_
#include "PDGNode.hh"
#include "PDGEdge.hh"
#include "PDGEnums.hh"
#include "PDGCommandLineOptions.hh"
#include "PDGCallGraph.hh"


#include <unordered_map>
#include <map>
#include <set>
using namespace pdg;


namespace qg
{
  // class Node;
  // class Edge;

  class GenericGraph
  {
  public:
      typedef std::unordered_map<std::string, Node *> StatementNodeMap;
      typedef std::unordered_map<std::string, Node *> FunLastNodeMap;
      typedef std::unordered_map<std::string, Node *> FunFirstNodeMap;
    typedef std::set<Edge *> EdgeSet;
    typedef std::set<Node *> NodeSet;
    StatementNodeMap::iterator st_node_map_begin() { return _st_node_map.begin(); }
    StatementNodeMap::iterator st_node_map_end() { return _st_node_map.end(); }
    GenericGraph() = default;
    NodeSet::iterator begin() { return _node_set.begin(); }
    NodeSet::iterator end() { return _node_set.end(); }
    NodeSet::iterator begin() const { return _node_set.begin(); }
    NodeSet::iterator end() const { return _node_set.end(); }
    virtual void build(llvm::Module &M) = 0;
    void addEdge(Edge &e) { _edge_set.insert(&e); }
    void addNode(Node &n) { _node_set.insert(&n); }
    Node *getNode(std::string str);
    bool hasNode(std::string str);
    int numEdge() { return _edge_set.size(); }
    int numNode() { return _st_node_map.size(); }
    void setIsBuild() { _is_build = true; }
    bool isBuild() { return _is_build; }
    bool canReach(pdg::Node &src, pdg::Node &dst);
    bool canReach(pdg::Node &src, pdg::Node &dst, std::set<pdg::EdgeType> exclude_edge_types);
    StatementNodeMap &getStNodeMap() { return _st_node_map; }
     FunLastNodeMap &getFunLastNodeMap() { return _fun_lastnode_map; }
     FunFirstNodeMap &getFunFirstNodeMap() { return _fun_firstnode_map; }
    void dumpGraph();

    // 返回边集合的引用
EdgeSet& getEdges() { return _edge_set; }

// 返回边集合的常量引用
const EdgeSet& getEdges() const { return _edge_set; }

// 提供边集合的迭代器
EdgeSet::iterator edge_begin() { return _edge_set.begin(); }
EdgeSet::iterator edge_end() { return _edge_set.end(); }
EdgeSet::const_iterator edge_begin() const { return _edge_set.begin(); }
EdgeSet::const_iterator edge_end() const { return _edge_set.end(); }

  protected:
    StatementNodeMap _st_node_map;
    FunLastNodeMap _fun_lastnode_map;//记录每个函数对应的最后一条语句节点
    FunFirstNodeMap _fun_firstnode_map; // 新增：记录函数第一个节点
    EdgeSet _edge_set;
    NodeSet _node_set;
    bool _is_build = false;
  };

  class QPgGraph : public GenericGraph
  {
  public:
    QPgGraph() = default;
    QPgGraph(const QPgGraph &) = delete;
    QPgGraph(QPgGraph &&) = delete;
    QPgGraph &operator=(const QPgGraph &) = delete;
    QPgGraph &operator=(QPgGraph &&) = delete;
    static QPgGraph &getInstance()
    {
      static QPgGraph g{};
      return g;
    }

    void build(llvm::Module &M);

    // 读取边权重文件
    std::unordered_map<std::string, std::unordered_map<std::string, int>> readEdgeWeights(const std::string& filename);
    
    // 根据节点获取边键
    std::string nodeToEdgeKey(pdg::Node* node);
    
    // 获取边权重
    int getEdgeWeight(pdg::Node* src, pdg::Node* dst, int defaultWeight = 0);
    
    // 获取正向边权重映射
    const std::unordered_map<std::string, std::unordered_map<std::string, int>>& getForwardEdgeWeights() const {
        return _forward_edge_weights;
    }
    
    // 获取反向边权重映射
    const std::unordered_map<std::string, std::unordered_map<std::string, int>>& getBackwardEdgeWeights() const {
        return _backward_edge_weights;
    }
private:
  // 边权重映射
  std::unordered_map<std::string, std::unordered_map<std::string, int>> _forward_edge_weights;
  std::unordered_map<std::string, std::unordered_map<std::string, int>> _backward_edge_weights;
  

  };

  class QUANProgramGraph: public llvm::ModulePass
  {
    public:
    static char ID;
    QUANProgramGraph() : llvm::ModulePass(ID) {};
      bool runOnModule(llvm::Module &M) override;
    QPgGraph *getQG() { return _QG; }

  private:
      QPgGraph *_QG;
  };
} // namespace pdg

#endif
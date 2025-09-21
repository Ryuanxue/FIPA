#ifndef PDGCALLGRAPH_H_
#define PDGCALLGRAPH_H_
#include "LLVMEssentials.hh"
#include "Graph.hh"
#include "PDGUtils.hh"

namespace pdg
{
  class PDGCallGraph : public GenericGraph
  {
  public:
  typedef std::unordered_map<std::string, std::string> File_Line_FunMap;//add by xue 记录call site的文件+fun+行号信息与callee函数映射
    using PathVecs = std::vector<std::vector<llvm::Function *>>;
    PDGCallGraph() = default;
    PDGCallGraph(const PDGCallGraph &) = delete;
    PDGCallGraph(PDGCallGraph &&) = delete;
    PDGCallGraph &operator=(const PDGCallGraph &) = delete;
    PDGCallGraph &operator=(PDGCallGraph &&) = delete;
    static PDGCallGraph &getInstance()
    {
      static PDGCallGraph g{};
      return g;
    }
    void build(llvm::Module &M) override;
    std::set<llvm::Function *> getIndirectCallCandidates(llvm::CallInst &ci, llvm::Module &M);
    bool isFuncSignatureMatch(llvm::CallInst &ci, llvm::Function &f);
    bool isTypeEqual(llvm::Type &t1, llvm::Type &t2);
    bool canReach(Node &src, Node &sink);
    void dump();
    void printPaths(Node &src, Node &sink);
    File_Line_FunMap Get_File_Line_FunMap(){return file_line_funMap;}//add by xue
    PathVecs computePaths(Node &src, Node &sink); // compute all pathes
    void computePathsHelper(PathVecs &path_vecs, Node &src, Node &sink, std::vector<llvm::Function *> cur_path, std::unordered_set<llvm::Function *> visited_funcs, bool &found_path);

  private:
  File_Line_FunMap file_line_funMap;
  };
} // namespace pdg

#endif
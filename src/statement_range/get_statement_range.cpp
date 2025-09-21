#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/AST/ParentMapContext.h> // 确保包含此头文件
#include <tinyxml2.h>
#include <iostream>
#include <fstream>

using namespace clang;
using namespace clang::tooling;
using namespace tinyxml2;

class FunctionVisitor : public RecursiveASTVisitor<FunctionVisitor> {
public:
    explicit FunctionVisitor(ASTContext *Context, XMLElement *xmlFunction)
        : Context(Context), xmlFunction(xmlFunction) {}

    bool VisitFunctionDecl(FunctionDecl *f) {
        if (f->hasBody()) {
            CompoundStmt *body = dyn_cast<CompoundStmt>(f->getBody());
            if (body) {
                for (auto stmt : body->body()) {
                    handleStmt(stmt);
                }
            }
        }
        return true;
    }

private:
    // void handleStmt(Stmt *s) {
    //     //
    //     FullSourceLoc FullLocation = Context->getFullLoc(s->getBeginLoc());
    //     if (FullLocation.isValid()) {
    //             int startLine = FullLocation.getSpellingLineNumber();
    //             int endLine = Context->getFullLoc(s->getEndLoc()).getSpellingLineNumber();
                
    //             // 根据语句类型生成 XML
    //             std::string stmtType = s->getStmtClassName();
    //             std::string lineRange = std::to_string(startLine) + ":" + std::to_string(endLine);

    //             XMLElement *xmlStmt = xmlFunction->GetDocument()->NewElement("stmt");
    //             xmlStmt->SetAttribute("dec", (stmtType + ":" + lineRange).c_str());
    //             xmlFunction->InsertEndChild(xmlStmt);
    //         }
    // }
    /**宏定义会导致语句的实际位置被替换为宏定义的位置。为了正确获取语句的原始位置（即出现在源文件中的位置），我们需要绕过宏替换的影响。

在 Clang 中，可以通过 SourceManager 和 FullSourceLoc 的 API 来获取语句的扩展位置（Expansion Location），而不是它的定义位置（Spelling Location） */

    void handleStmt(Stmt *s) {
        const SourceManager &SM = Context->getSourceManager();
        FullSourceLoc FullLocation = Context->getFullLoc(s->getBeginLoc());
        
        // 使用 ExpansionLoc 获取宏展开后的实际位置
        SourceLocation startLoc = SM.getExpansionLoc(s->getBeginLoc());
        SourceLocation endLoc = SM.getExpansionLoc(s->getEndLoc());
        
        // 获取行号
        int startLine = SM.getSpellingLineNumber(startLoc);
        int endLine = SM.getSpellingLineNumber(endLoc);

        if (FullLocation.isValid()) {
            // 根据语句类型生成 XML
            std::string stmtType = s->getStmtClassName();
            std::string lineRange = std::to_string(startLine) + ":" + std::to_string(endLine);

            XMLElement *xmlStmt = xmlFunction->GetDocument()->NewElement("stmt");
            xmlStmt->SetAttribute("dec", (stmtType + ":" + lineRange).c_str());
            xmlFunction->InsertEndChild(xmlStmt);
        }
    }


    ASTContext *Context;
    XMLElement *xmlFunction;
};

class FunctionASTConsumer : public ASTConsumer {
public:
    explicit FunctionASTConsumer(ASTContext *Context, XMLDocument &xmlDoc)
        : Visitor(Context, nullptr), xmlDoc(xmlDoc) {}

    void HandleTranslationUnit(ASTContext &Context) override {
        TranslationUnitDecl *tu = Context.getTranslationUnitDecl();
        for (Decl *decl : tu->decls()) {
            if (FunctionDecl *funcDecl = dyn_cast<FunctionDecl>(decl)) {
                // 忽略只有声明没有定义的函数
                if (!funcDecl->hasBody()) {
                    continue;
                }

                XMLElement *xmlFunction = xmlDoc.NewElement("function");
                xmlFunction->SetAttribute("name", funcDecl->getNameInfo().getName().getAsString().c_str());

                // 获取函数所在源文件的绝对路径
                SourceManager &SM = Context.getSourceManager();
                std::string sourceFilePath = SM.getFilename(funcDecl->getBody()->getBeginLoc()).str();
                char absPath[PATH_MAX];
                if (realpath(sourceFilePath.c_str(), absPath)) {
                    xmlFunction->SetAttribute("dir", absPath);
                }

                // 将 xmlFunction 传递给 Visitor
                Visitor = FunctionVisitor(&Context, xmlFunction);
                Visitor.TraverseDecl(funcDecl);
                // 访问函数体
                    // Visitor.TraverseStmt(funcDecl->getBody());

                xmlDoc.RootElement()->InsertEndChild(xmlFunction);
            }
        }
}


private:
    FunctionVisitor Visitor;
    XMLDocument &xmlDoc;
};

class FunctionFrontendAction : public ASTFrontendAction {
public:
    FunctionFrontendAction() {
        // 创建 XML 根节点
        xmlRoot = xmlDoc.NewElement("root");
        xmlDoc.InsertFirstChild(xmlRoot);
    }

      std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) override {
        sourceFilePath = file.str();
        return std::make_unique<FunctionASTConsumer>(&CI.getASTContext(), xmlDoc);
    }

    void EndSourceFileAction() override {
        // 获取文件名（不含路径和扩展名）
        std::string filename = llvm::sys::path::stem(sourceFilePath).str();
        // 设置输出XML文件名
        std::string xmlFilename = filename + ".xml";
        xmlDoc.SaveFile(xmlFilename.c_str());
        std::cout << "XML saved to " << xmlFilename << std::endl;
    }

private:
    XMLDocument xmlDoc;
    XMLElement *xmlRoot;
    std::string sourceFilePath;
};

static llvm::cl::OptionCategory ToolCategory("Tool Options");

int main(int argc, const char **argv) {
    CommonOptionsParser OptionsParser(argc, argv, ToolCategory);
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
    
    // 添加编译选项以忽略未使用参数警告
    Tool.appendArgumentsAdjuster(
        [](const CommandLineArguments &args, StringRef filename) {
            CommandLineArguments adjustedArgs(args);
            adjustedArgs.push_back("-Wno-unused-parameter");
            adjustedArgs.push_back("-Wno-unused-variable");
            adjustedArgs.push_back("-Wno-error");
            adjustedArgs.push_back("-Wno-implicit-function-declaration");
            return adjustedArgs;
        }
    );
    
    return Tool.run(newFrontendActionFactory<FunctionFrontendAction>().get());
}

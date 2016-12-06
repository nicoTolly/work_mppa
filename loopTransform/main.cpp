#include <iostream>


#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace std;
using namespace clang;
using namespace clang::tooling;
using namespace llvm;

static cl::OptionCategory toolCat("my options");

int main( int argc, const char **argv)
{
  CommonOptionsParser op(argc, argv, toolCat);
  ClangTool Tool(op.getCompilations(), op.getSourcePathList());
  return 0;
}


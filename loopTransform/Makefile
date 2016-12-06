#change these variable to local settings
LLVM_BUILD_DIR=/home/nico/stage/build
LLVM_SRC_DIR=/home/nico/stage/llvm

#needs the llvm-config tool in path
LLVM_CXX_FLAGS=`llvm-config --cxxflags`
LLVM_LFLAGS=`llvm-config --ldflags --libs --system-libs`
CXX_FLAGS=-fno-rtti -O1
INCLUDE_FLAGS=-I$(LLVM_BUILD_DIR)/tools/clang/include -I$(LLVM_BUILD_DIR)/tools/clang/include
LFLAGS=-Wl,--start-group -lclangAST -lclangASTMatchers -lclangAnalysis -lclangBasic -lclangDriver -lclangEdit -lclangFrontend -lclangFrontendTool -lclangLex -lclangParse -lclangSema -lclangEdit -lclangRewrite -lclangRewriteFrontend -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangSerialization -lclangToolingCore -lclangTooling -lclangFormat -Wl,--end-group 

CC=g++

BUILD=build

SRC=main.cpp
BIN=$(BUILD)/transform

.PHONY=all

all: makedir $(BIN)

makedir:
	test -d $(BUILD) || mkdir $(BUILD)

$(BUILD)/transform: $(BUILD)/main.o
	$(CC) $(CXX_FLAGS) $(LLVM_CXX_FLAGS) $(INCLUDE_FLAGS) $^ -o $@ $(LFLAGS) $(LLVM_LFLAGS)

$(BUILD)/%.o: %.cpp
	$(CC) $(CXX_FLAGS) $(LLVM_CXX_FLAGS) $(INCLUDE_FLAGS) -c $^ -o $@ 

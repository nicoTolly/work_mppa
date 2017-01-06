# Must create a file named Makefile.local and define
# LLVM_BUILD_DIR and LLVM_SRC_DIR according to local settings
# for example:
# LLVM_BUILD_DIR =/home/nico/stage/build
# LLVM_SRC_DIR =/home/nico/stage/llvm

#look for LLVM_BUILD_DIR and LLVM_SRC_DIR in Makefile.local
#abort if this file does not exist
ifneq ("$(wildcard Makefile.local)","")
  include Makefile.local
else
  $(error must define local LLVM_BUILD_DIR and LLVM_SRC_DIR in Makefile.local)
endif

ifndef LLVM_BUILD_DIR
  $(error LLVM_BUILD_DIR not defined)
endif

ifndef LLVM_SRC_DIR
  $(error LLVM_SRC_DIR not defined)
endif

#needs the llvm-config tool in path
LLVM_CXX_FLAGS=`llvm-config --cxxflags`
LLVM_LFLAGS=`llvm-config --ldflags --libs --system-libs`
CXX_FLAGS=-fno-rtti -O1 -std=c++11
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
	$(CC) $(CXX_FLAGS) $(LLVM_CXX_FLAGS)  $^ -o $@ $(LFLAGS) $(LLVM_LFLAGS)

#Include in this line. Seems to be version compatibility problems between my home built clang and the version 
#from repository
	#$(CC) $(CXX_FLAGS) $(LLVM_CXX_FLAGS) $(INCLUDE_FLAGS) $^ -o $@ $(LFLAGS) $(LLVM_LFLAGS)

$(BUILD)/%.o: %.cpp
	$(CC) $(CXX_FLAGS) $(LLVM_CXX_FLAGS)  -c $^ -o $@ 

	#$(CC) $(CXX_FLAGS) $(LLVM_CXX_FLAGS) $(INCLUDE_FLAGS) -c $^ -o $@ 

.PHONY=clean

clean:
	rm -fr build/

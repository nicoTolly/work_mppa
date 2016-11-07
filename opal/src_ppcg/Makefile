ARCH ?= CPU
# ARCH ?= MPPA

PPCG_ROOT ?= $(HOME)/build
# # PPCG : check if PATH to PPCG is set
# ifeq ($(PPCG_BIN_DIR),)
# $(error PPCG_BIN_DIR is not set. Please set this (e.g /dir/to/ppcg_bin/) )
# endif

CC = gcc
CFLAGS = -O2 -std=c99 -g -Wall -DSRC_DIR=\"$(shell pwd)/\" -D$(ARCH) 
# add flags to avoid error on unknown clock_gettime() on C99
CFLAGS += -D_POSIX_C_SOURCE=199309L

# for server with GPU and MIC with ocl-icd installed on /usr/local/lib
LFLAGS =  -Wl,-rpath,/usr/lib/x86_64-linux-gnu/beignet -lm -lrt -ljansson -L/usr/lib/x86_64-linux-gnu/beignet -lOpenCL

ifeq ($(ARCH), CUDA)
# CFLAGS += -I/opt/cuda/include
else ifeq ($(ARCH), MIC)
# CC = icc
CC = gcc
# CFLAGS += -I$(HOME)/jansson/include -I..
else ifeq ($(ARCH), CPU)
CC = gcc
CFLAGS += -I..
# LFLAGS = -L/opt/intel/opencl/lib64 -Wl,-rpath -Wl,/opt/intel/opencl/lib64 -lm -lrt -ljansson -lOpenCL
else ifeq ($(ARCH), MPPA)
CFLAGS += $(shell k1-pkg-config --cflags opencl)
LFLAGS = -lm -lrt -ljansson $(shell k1-pkg-config --libs opencl)
endif

BIN = ../test/opal_ppcg_$(ARCH)
DEPEND = ../build/depend
# SRC = $(wildcard *.c)

# PPCG_SRC_DIR = ./ppcg_S_32
PPCG_SRC_DIR = .

SRC = export.c results.c data.c config.c print.c utils.c 
ifeq ($(ARCH), CUDA)
SRC += ocl_utilities.c $(PPCG_SRC_DIR)/main_kernel.c $(PPCG_SRC_DIR)/main_host.c 
else ifeq ($(ARCH), MIC)
SRC += ocl_utilities.c $(PPCG_SRC_DIR)/main_kernel.c $(PPCG_SRC_DIR)/main_host.c 
else ifeq ($(ARCH), MPPA)
SRC += ocl_utilities.c $(PPCG_SRC_DIR)/main_kernel.c $(PPCG_SRC_DIR)/main_host.c 
else ifeq ($(ARCH), CPU)
# SRC += main.ppcg.c 
SRC += main.c 
endif

OBJ = $(patsubst %.c,../build/ppcg/%.o,$(SRC))

# all: clean outdir ppcg $(BIN) 
all: clean outdir $(BIN) 
	# ln -sf $(PPCG_SRC_DIR)/main_kernel.cl main_kernel.cl

$(DEPEND): $(SRC)
	$(CC) -MM $^ > $@

ifndef $@
-include $(DEPEND)
endif

../build/ppcg/%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

outdir:
	mkdir -p ../build/ppcg ../build/ppcg/$(PPCG_SRC_DIR)

$(BIN): $(OBJ)
	$(CC) $^ -o $@ $(LFLAGS) 

ppcg:
	test -s $(PPCG_ROOT)/bin/ppcg || { echo "[PPCG] Please set PPCG_ROOT by make PPCG_ROOT=... " ; exit 1;}
	test -s main_host.c || { echo "[PPCG] Generating from main.c"; $(PPCG_ROOT)/bin/ppcg main.c --target=opencl -S 8;}
	# test -s ../main_kernel.cl || { echo "[PPCG] Creating symlink to main_kernel.cl"; ln -s src_ppcg/main_kernel.cl ../main_kernel.cl;}
	test -s ocl_utilities.c || { echo "[PPCG] Creating symlink to ocl_utilities.c"; ln -s $(PPCG_ROOT)/share/ppcg/ocl_utilities.c ocl_utilities.c;}

.PHONY: clean

clean:
	$(RM) $(BIN) $(OBJ) $(DEPEND)
	touch -t197001010100 $(DEPEND)


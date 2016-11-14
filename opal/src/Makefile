# ARCH ?= CPU
#ARCH ?= CUDA
#ARCH ?= MIC
# ARCH ?= MPPA

CC = gcc
CFLAGS = -O2 -g -Wall -I.. -DSRC_DIR=\"$(shell pwd)/\" -D$(ARCH) $(OPTS)

# for server with GPU and MIC with ocl-icd installed on /usr/local/lib
LFLAGS = -lm -lrt -ljansson -lOpenCL

ifeq ($(ARCH), CUDA)
	CFLAGS += -I/opt/cuda/include
else ifeq ($(ARCH), MIC)
	# CC = icc
	CC = gcc
	CFLAGS += -I$(HOME)/jansson/include -I..
else ifeq ($(ARCH), CPU)
	CC = gcc
	CFLAGS += -I..
	# LFLAGS = -L/opt/intel/opencl/lib64 -Wl,-rpath -Wl,/opt/intel/opencl/lib64 -lm -lrt -ljansson -lOpenCL
else ifeq ($(ARCH), MPPA)
	CFLAGS += $(shell k1-pkg-config --cflags opencl)
	LFLAGS = -lm -lrt -ljansson $(shell k1-pkg-config --libs opencl)
endif

BIN = ../test/opal_$(ARCH)
DEPEND = ../build/depend
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,../build/%.o,$(SRC))

all: $(BIN)

$(DEPEND): $(SRC)
	$(CC) -MM $^ > $@

ifndef $@
-include $(DEPEND)
endif

../build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(LFLAGS) $^ -o $@

.PHONY: clean

clean:
	$(RM) $(BIN) $(OBJ) $(DEPEND)
	touch -t197001010100 $(DEPEND)


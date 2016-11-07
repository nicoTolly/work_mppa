ARCH ?= CPU
# ARCH ?= CUDA
# ARCH ?= MIC
# ARCH ?= MPPA

CC = gcc
CFLAGS = -O2 -std=c99 -g -Wall -DSRC_DIR=\"$(shell pwd)/\" -D$(ARCH)
LFLAGS = -lm -lrt -ljansson

BIN = ../test/opal_mono
DEPEND = ../build/depend
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,../build/mono/%.o,$(SRC))

all: outdir $(BIN)

$(DEPEND): $(SRC)
	$(CC) -MM $^ > $@

ifndef $@
-include $(DEPEND)
endif

../build/mono/%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

outdir:
	mkdir -p ../build/mono

$(BIN): $(OBJ)
	$(CC)  $^ -o $@ $(LFLAGS)

.PHONY: clean

clean:
	$(RM) $(BIN) $(OBJ) $(DEPEND)
	touch -t197001010100 $(DEPEND)


CC=gcc
DEBUG=yes


# -foptimize-sibling-calls may cause bugs (or not) 
#  To be tested
CFLAGS=-Wall 
ifeq ($(DEBUG),yes)
	CFLAGS+= -g 
	EXEC= isl_hello.db
else
	CFLAGS+= -O3
	EXEC= isl_hello
endif



SRC= main.c schedule_visitor.c isl_utils.c node_count.c
LDFLAGS= -lisl -lpet -lpthread


PROJECT_ROOT_DIR= $(shell pwd)


DEBUG_OBJECTS = $(addprefix $(PROJECT_ROOT_DIR)/debug_obj/,$(SRC:.c=.o))
RELEASE_OBJECTS = $(addprefix $(PROJECT_ROOT_DIR)/release_obj/,$(SRC:.c=.o))


.PHONY: all
.PHONY: dir
.PHONY: depend
.PHONY: debug
.PHONY: clean

all: dir depend $(EXEC)

dir:
	if ! test -d $(PROJECT_ROOT_DIR)/release_obj  ; then mkdir release_obj; fi
	if  ! test -d $(PROJECT_ROOT_DIR)/debug_obj  ; then mkdir debug_obj; fi

debug: $(EXECD)
	$(CFLAGS)+=-g



isl_hello.db:$(DEBUG_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

isl_hello: $(RELEASE_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(PROJECT_ROOT_DIR)/release_obj/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


$(PROJECT_ROOT_DIR)/debug_obj/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

depend:.depend

.depend: $(SRC)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ | sed -e "s:\(.*\.o\):$(PROJECT_ROOT_DIR)/release_obj/\1:g" >> ./.depend
	$(CC) $(CFLAGS) -MM $^ | sed -e "s:\(.*\.o\):$(PROJECT_ROOT_DIR)/debug_obj/\1:g" >> ./.depend


include .depend


clean:
	rm -fr $(PROJECT_ROOT_DIR)/release_obj/*.o
	rm -fr $(PROJECT_ROOT_DIR)/debug_obj/*.o

mrproper: clean
	rm -fr isl_hello isl_hello


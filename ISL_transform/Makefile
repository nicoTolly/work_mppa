CC=gcc
DEBUG=yes

ifeq ($(DEBUG),yes)
	CXXFLAGS=-Wall -g 
	EXEC= isl_hello.db
else
	CXXFLAGS=-Wall -O3
	EXEC= isl_hello
endif



SRC= main.c schedule_visitor.c isl_utils.c
LDFLAGS= -lisl -lpet


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
	$(CXXFLAGS)+=-g



isl_hello.db:$(DEBUG_OBJECTS)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

isl_hello: $(RELEASE_OBJECTS)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(PROJECT_ROOT_DIR)/release_obj/%.o: %.c
	$(CC) $(CXXFLAGS) -o $@ -c $<


$(PROJECT_ROOT_DIR)/debug_obj/%.o: %.c
	$(CC) $(CXXFLAGS) -o $@ -c $<

depend:.depend

.depend: $(SRC)
	rm -f ./.depend
	$(CC) $(CXXFLAGS) -MM $^ | sed -e "s:\(.*\.o\):$(PROJECT_ROOT_DIR)/release_obj/\1:g" >> ./.depend
	$(CC) $(CXXFLAGS) -MM $^ | sed -e "s:\(.*\.o\):$(PROJECT_ROOT_DIR)/debug_obj/\1:g" >> ./.depend


include .depend


clean:
	rm -fr $(PROJECT_ROOT_DIR)/release_obj/*.o
	rm -fr $(PROJECT_ROOT_DIR)/debug_obj/*.o

mrproper: clean
	rm -fr isl_hello isl_hello


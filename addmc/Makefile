## src inclusion chain: logic.h in (dmc|htb).h

# opt = -Ofast # be careful with inf and nan
ASSEMBLY_OPTIONS = -g -std=c++2a -Wno-register $(opt)

# link = -static # beware of segfault with std::thread
LINK_OPTIONS = -lgmpxx -lgmp $(link) # order matters

################################################################################

CUDD_DIR = libraries/cudd
CUDD_LIB_DIR = $(CUDD_DIR)/cudd/.libs
CUDD_TARGET = $(CUDD_LIB_DIR)/libcudd.a
CUDD_INCLUSIONS = -I$(CUDD_DIR) -I$(CUDD_DIR)/cudd -I$(CUDD_DIR)/epd -I$(CUDD_DIR)/mtr -I$(CUDD_DIR)/st
CUDD_LINKS = -L$(CUDD_LIB_DIR) -lcudd

SYLVAN_DIR = libraries/sylvan
SYLVAN_BUILD_DIR = $(SYLVAN_DIR)/build
SYLVAN_LIB_DIR = $(SYLVAN_BUILD_DIR)/src
SYLVAN_TARGET = $(SYLVAN_LIB_DIR)/libsylvan.a
SYLVAN_INCLUSIONS = -I$(SYLVAN_DIR)/src
SYLVAN_LINKS = -L$(SYLVAN_LIB_DIR) -lsylvan

CXXOPTS = libraries/cxxopts/include/cxxopts.hpp

################################################################################

DMC_OBJECTS = logic.o dmc.o
HTB_OBJECTS = logic.o htb.o

.ONESHELL: # applies to all targets

dmc: $(DMC_OBJECTS)
	g++ -o dmc $(DMC_OBJECTS) $(CUDD_LINKS) $(SYLVAN_LINKS) $(LINK_OPTIONS)

htb: $(HTB_OBJECTS)
	g++ -o htb $(HTB_OBJECTS) $(LINK_OPTIONS)

dmc.o: src/dmc.cc src/dmc.hh src/logic.hh $(CUDD_TARGET) $(SYLVAN_TARGET) $(CXXOPTS)
	g++ src/dmc.cc -c $(CUDD_INCLUSIONS) $(SYLVAN_INCLUSIONS) $(ASSEMBLY_OPTIONS)

htb.o: src/htb.cc src/htb.hh src/logic.hh $(CXXOPTS)
	g++ src/htb.cc -c $(ASSEMBLY_OPTIONS)

logic.o: src/logic.cc src/logic.hh
	g++ src/logic.cc -c $(ASSEMBLY_OPTIONS)

$(CUDD_TARGET): $(shell find $(CUDD_DIR)/cudd -name "*.c" -o -name "*.h") $(shell find $(CUDD_DIR)/cplusplus -name "*.cc" -o -name "*.hh")
	cd $(CUDD_DIR)
	./INSTALL.sh

$(SYLVAN_TARGET): $(shell find $(SYLVAN_DIR)/src -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp")
	mkdir -p $(SYLVAN_BUILD_DIR)
	cd $(SYLVAN_BUILD_DIR)
	cmake .. -DBUILD_SHARED_LIBS=off
	make -s

.PHONY: all cudd sylvan clean clean-cudd clean-sylvan clean-libraries clean-all

all: dmc htb

cudd: $(CUDD_TARGET)

sylvan: $(SYLVAN_TARGET)

clean:
	rm -f *.o dmc htb

clean-cudd:
	cd $(CUDD_DIR) && git clean -xdf

clean-sylvan:
	rm -rf $(SYLVAN_BUILD_DIR)

clean-libraries: clean-cudd clean-sylvan

clean-all: clean clean-libraries

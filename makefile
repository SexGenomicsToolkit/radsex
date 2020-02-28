# Compiler options
CC = g++
OPTCFLAGS = -O2
CFLAGS = -Wall -Wno-maybe-uninitialized -std=c++11 $(OPTCFLAGS)
LDFLAGS = -pthread -lstdc++ -lz -L$(INCLUDE)/bwa -lbwa

# Directory organisation
BASEDIR = .
BIN = $(BASEDIR)/bin
BUILD = $(BASEDIR)/build
INCLUDE = $(BASEDIR)/include
SRC = $(BASEDIR)/src
CPP = $(wildcard $(SRC)/*.cpp)

# Get number of parallel jobs
MAKE_PID := $(shell echo $$PPID)
JOBS := $(shell ps T | sed -n 's/.*$(MAKE_PID).*$(MAKE).* \(-j\|--jobs\) *\([0-9][0-9]*\).*/\2/p')
ifeq ($(JOBS),)
	JOBS := 1
endif

# Object files inferred from cpp files
OBJS = $(addprefix $(BUILD)/, $(notdir $(CPP:.cpp=.o)))

# Target
TARGETS = $(BIN)/radsex

# Declare phony targets (i.e. targets which are not files)
.PHONY: all clean clean-bwa clean-kfun clean-all rebuild rebuild-all

# Main rule
all: $(BUILD) $(BIN) $(TARGETS)

# Build directory
$(BUILD):
	if [ ! -e $@ ]; then \
		mkdir $@;        \
	fi;

# Bin directory
$(BIN):
	if [ ! -e $@ ]; then \
		mkdir $@;        \
	fi;

# Build BWA
$(INCLUDE)/bwa/libbwa.a:
	(cd $(INCLUDE)/bwa && $(MAKE) -j $(JOBS))

# Clean BWA
clean-bwa:
	(cd $(INCLUDE)/bwa && $(MAKE) clean)

# Build kfun
$(INCLUDE)/kfun/kfun.o: $(INCLUDE)/kfun/kfun.cpp
	$(CC) $(CFLAGS) -I $(INCLUDE) -c -o $@ $^

# Clean BWA
clean-kfun:
	(rm $(INCLUDE)/kfun/kfun.o)

# Clean RADSex files
clean:
	rm -rf $(BUILD)/*.o
	rm -rf $(BIN)/*

# Clean all files
clean-all: clean clean-bwa clean-kfun

# Rebuild RADSex only
rebuild:
	$(MAKE) clean
	$(MAKE) -j $(JOBS)

# Rebuild RADSex and dependencies
rebuild-all:
	$(MAKE) clean-all
	$(MAKE) -j $(JOBS)

# Linking
# $(BIN)/radsex: $(OBJS) $(INCLUDE)/kfun/kfun.o
$(BIN)/radsex: build/stats.o build/main.o build/analysis.o build/marker.o build/popmap.o build/markers_table.o build/depth.o $(INCLUDE)/kfun/kfun.o
	$(CC) $(CFLAGS) -I $(INCLUDE) -o $(BIN)/radsex $^ $(LDFLAGS)

# Build a single object file. Added libs as dependency so they are built before object files
$(BUILD)/%.o: $(SRC)/%.cpp $(INCLUDE)/bwa/libbwa.a
	$(CC) $(CFLAGS) -I $(INCLUDE) -c -o $@ $<



# Compiler options
CC = g++
OPTCFLAGS = -Ofast
CFLAGS = -Wall -std=c++11 $(OPTCFLAGS)
LDFLAGS = -pthread -static-libstdc++ -lz

# Directory organisation
BASEDIR = .
BIN = $(BASEDIR)/bin
BUILD = $(BASEDIR)/build
LIBBUILD = $(BASEDIR)/build
INCLUDE = $(BASEDIR)/include
SRC = $(BASEDIR)/src
CPP = $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/*/*.cpp)
LIBCPP = $(wildcard $(INCLUDE)/*/*.cpp)

# Target
TARGET = radsex

# Variables
OBJS = $(addprefix $(BUILD)/, $(notdir $(CPP:.cpp=.o)))
LIBOBJS = $(addprefix $(LIBBUILD)/, $(notdir $(LIBCPP:.cpp=.o)))

# Rules

all: init print-OBJS print-LIBOBJS $(TARGET)

print-%  : ; @echo $* = $($*)

$(TARGET): $(OBJS) $(LIBOBJS)
	$(CC) $(CFLAGS) -I $(INCLUDE) -o $(BIN)/$(TARGET) $^ $(LDFLAGS)

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -I $(INCLUDE) -c -o $@ $^

$(LIBBUILD)/%.o: $(INCLUDE)/*/%.cpp
	$(CC) $(CFLAGS) -I $(INCLUDE) -c -o $@ $^

clean:
	@rm -rf $(BUILD)/*.o
	@rm -rf $(BIN)/$(TARGET)

init:
	@mkdir -p $(BUILD) $(BUILD)
	@mkdir -p $(BIN) $(BIN)

rebuild: clean $(TARGET)

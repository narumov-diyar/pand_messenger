CXX = g++
CXXFLAGS = -Wall -I include

SRC = src/client.cpp src/errors.cpp src/general_funcs.cpp 

BIN_DIR = build
BIN = $(BIN_DIR)/app

all: $(BIN)
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN) : $(SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@;

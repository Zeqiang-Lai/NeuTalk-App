EXE_MAIN = bin/client

SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

MAIN_PATH = cli/main.cpp

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CFLAGS = -g -Wall
CPPFLAGS = -I include -std=c++11

main: $(EXE_MAIN)

all: main

$(EXE_MAIN): $(BIN_DIR) $(OBJ_DIR) $(OBJ) $(MAIN_PATH)
	$(CXX) $(CPPFLAGS) $(OBJ) $(MAIN_PATH) -lpthread -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -lpthread -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(RM) $(OBJ) $(EXE_MAIN)

.PHONY: all clean
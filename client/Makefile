EXE_MAIN = bin/neutalk
EXE_CONSOLE = bin/consoletalk

CORE_SRC_DIR = core/src
UI_SRC_DIR = ui/src

CORE_OBJ_DIR = build/core
UI_OBJ_DIR = build/ui

BIN_DIR = bin

MAIN_PATH = ui/cli/main.cpp
CONSOLE_PATH = console/cli/main.cpp

CORE_SRC = $(wildcard $(CORE_SRC_DIR)/*.cpp)
CORE_OBJ = $(CORE_SRC:$(CORE_SRC_DIR)/%.cpp=$(CORE_OBJ_DIR)/%.o)

UI_SRC = $(wildcard $(UI_SRC_DIR)/*.cpp)
UI_OBJ = $(UI_SRC:$(UI_SRC_DIR)/%.cpp=$(UI_OBJ_DIR)/%.o)

CFLAGS = -g -Wall
CPPFLAGS = -I core/include -I ui/include -std=c++11

gtk: $(EXE_MAIN)

console: $(EXE_CONSOLE)

all: gtk console

$(EXE_MAIN): $(BIN_DIR) $(CORE_OBJ_DIR) $(UI_OBJ_DIR) $(CORE_OBJ) $(UI_OBJ) $(MAIN_PATH)
	$(CXX) $(CPPFLAGS) $(CORE_OBJ) $(UI_OBJ) $(MAIN_PATH) -lpthread -o $@ `pkg-config gtkmm-3.0 --cflags --libs`

$(EXE_CONSOLE): $(BIN_DIR) $(CORE_OBJ_DIR) $(CORE_OBJ) $(CONSOLE_PATH)
	$(CXX) $(CPPFLAGS) $(CORE_OBJ) $(CONSOLE_PATH) -lpthread -o $@

$(CORE_OBJ_DIR)/%.o: $(CORE_SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -lpthread -o $@

$(UI_OBJ_DIR)/%.o: $(UI_SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ `pkg-config gtkmm-3.0 --cflags --libs`

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
$(CORE_OBJ_DIR):
	mkdir -p $(CORE_OBJ_DIR)
$(UI_OBJ_DIR):
	mkdir -p $(UI_OBJ_DIR)

clean:
	$(RM) $(CORE_OBJ) $(UI_OBJ) $(EXE_MAIN) $(EXE_CONSOLE)

.PHONY: all clean

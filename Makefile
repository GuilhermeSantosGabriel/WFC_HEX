UNAME_S := $(shell uname -s)

CXX      := g++
CC       := gcc
CXXFLAGS := -std=c++17 -O0 -Wall -g -MMD -MP

# --- OS Specific Configuration ---
ifeq ($(UNAME_S),Linux)
    LIBS := -lglfw -lGL -ldl -lpthread -lX11 -lXrandr -lXi
endif
ifeq ($(UNAME_S),Darwin)
    GLFW_PREFIX := /opt/homebrew/opt/glfw
    LIBS := -L$(GLFW_PREFIX)/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    INCLUDES += -I$(GLFW_PREFIX)/include
    CXXFLAGS += -stdlib=libc++
endif

# --- Paths ---
ROOT_DIR := WFC_Core
SRC_DIR  := $(ROOT_DIR)/src
INC_DIR  := $(ROOT_DIR)/include
EXTERNAL_DIR := $(ROOT_DIR)/external
BIN_DIR  := bin
OBJ_DIR  := obj
INCLUDES := -I$(ROOT_DIR) \
			-I$(INC_DIR) \
			-I$(EXTERNAL_DIR) \
			-I$(EXTERNAL_DIR)/imgui \
			-I$(EXTERNAL_DIR)/glad

# --- Source Files Selection ---
# Automatically find all .cpp in engine/, models/ and cli/ (or any subfolder except tests)
CORE_SRCS_CPP := $(shell find $(SRC_DIR) -name "*.cpp" ! -path "$(SRC_DIR)/tests/*" ! -name "main.cpp")
EXTERNAL_SRCS_CPP := $(shell find $(EXTERNAL_DIR) -name "*.cpp")
ALL_SRCS_CPP := $(CORE_SRCS_CPP) $(EXTERNAL_SRCS_CPP)

CORE_SRCS_C   := $(shell find $(EXTERNAL_DIR) -name "*.c")

MAIN_SRC     := $(SRC_DIR)/main.cpp
TEST_SRCS    := $(shell find $(SRC_DIR)/tests -name "*.cpp")

# --- Object Mapping ---
CORE_OBJS := $(CORE_SRCS_CPP:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
             $(EXTERNAL_SRCS_CPP:$(EXTERNAL_DIR)/%.cpp=$(OBJ_DIR)/external/%.o) \
             $(CORE_SRCS_C:$(EXTERNAL_DIR)/%.c=$(OBJ_DIR)/external/%.o)

MAIN_OBJ  := $(OBJ_DIR)/main.o
TEST_BINARIES := $(TEST_SRCS:$(SRC_DIR)/tests/%.cpp=$(BIN_DIR)/test_%)

# --- Default CLI Parameters ---
radius ?= 30
render ?= false
step   ?= 100
args   ?= 
name   ?=

# --- Primary Targets ---
TARGET = $(BIN_DIR)/wfc

all: $(TARGET)

$(TARGET): $(CORE_OBJS) $(MAIN_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)
	@echo "Build successful: $(TARGET)"

# --- Compilation Rules ---
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/external/%.o: $(EXTERNAL_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/external/%.o: $(EXTERNAL_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) -c $< -o $@

-include $(shell find $(OBJ_DIR) -name "*.d" 2>/dev/null)

# --- Execution Targets ---
run: all
	@./tools/run.sh $(radius) $(render) $(step) $(args)

headless: all
	@./tools/run.sh $(radius) false $(step) $(args)

# --- Architecture & Dependency Targets ---
view-deps:
	@./tools/dependencies/quick-view.sh

# --- Testing Targets ---
tests: $(TEST_BINARIES)
	@echo "All tests compiled in $(BIN_DIR)/"

$(BIN_DIR)/test_%: $(OBJ_DIR)/tests/%.o $(CORE_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

test: tests
	@if [ -z "$(name)" ]; then \
		./tools/run_all_tests.sh --map-radius $(radius) $(args); \
	else \
		./tools/run_test.sh $(name) --map-radius $(radius) $(args); \
	fi

# --- Maintenance ---
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
	@echo "Cleanup complete."

.PHONY: all clean tests run headless test view-deps help

# --- Help / Documentation ---
help:
	@echo "========================================================================"
	@echo "WFC HEX ENGINE - Build System"
	@echo "========================================================================"
	@echo "Usage: make [target] [variables]"
	@echo ""
	@echo "Main Targets:"
	@echo "  all             Build the main application (default)"
	@echo "  run             Build and run with current params (interactive/visual)"
	@echo "  headless        Build and run without OpenGL (useful for scripts)"
	@echo "  clean           Remove all binaries and object files"
	@echo ""
	@echo "Architecture Targets:"
	@echo "  view-deps       Generate and open the dynamic UML dependency graph"
	@echo ""
	@echo "Testing Targets:"
	@echo "  tests           Compile all test binaries"
	@echo "  test            Run all tests (or a specific one if 'name' is set)"
	@echo ""
	@echo "Configuration Variables (pass as var=value):"
	@echo "  radius          Map radius size (default: 30)"
	@echo "  render          Enable/Disable OpenGL rendering (true/false)"
	@echo "  step            Number of steps between frames (default: 100)"
	@echo "  name            Specific test name to run (e.g., make test name=perlin)"
	@echo "  args            Additional CLI arguments for the engine"
	@echo ""
	@echo "Examples:"
	@echo "  make run radius=50 render=true"
	@echo "  make test name=river radius=20 args='--river-seed 42'"
	@echo "  make view-deps"
	@echo "========================================================================"

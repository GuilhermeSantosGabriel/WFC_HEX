CXX      := g++
CC       := gcc
CXXFLAGS := -std=c++17 -O0 -Wall -g -MMD -MP
LIBS     := -lglfw -lGL -ldl -lpthread -lX11 -lXrandr -lXi

# Paths
ROOT_DIR := WFC_Core
SRC_DIR  := $(ROOT_DIR)/src
INC_DIR  := $(ROOT_DIR)/include
BIN_DIR  := bin
OBJ_DIR  := obj
INCLUDES := -I$(INC_DIR)

# Source Files
CORE_SRCS_CPP := $(shell find $(SRC_DIR)/engine $(SRC_DIR)/models -name "*.cpp")
CORE_SRCS_C   := $(SRC_DIR)/external/glad.c

MAIN_SRC     := $(SRC_DIR)/main.cpp
TEST_SRCS    := $(shell find $(SRC_DIR)/tests -name "*.cpp")

TEST_RUNNERS := $(TEST_SRCS)

CORE_OBJS := $(CORE_SRCS_CPP:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
             $(CORE_SRCS_C:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

MAIN_OBJ     := $(OBJ_DIR)/main.o
TEST_HELPERS_OBJ := $(OBJ_DIR)/tests/test_utils.o
TEST_BINARIES := $(TEST_RUNNERS:$(SRC_DIR)/tests/%.cpp=$(BIN_DIR)/test_%)

# Targets
TARGET = $(BIN_DIR)/wfc

all: $(TARGET)

$(TARGET): $(CORE_OBJS) $(MAIN_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)
	@echo "Built successfully: $(TARGET)"

tests: $(TEST_BINARIES)
	@echo "All tests compiled in $(BIN_DIR)/"

$(BIN_DIR)/test_%: $(OBJ_DIR)/tests/%.o $(CORE_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) -c $< -o $@

-include $(shell find $(OBJ_DIR) -name "*.d" 2>/dev/null)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
	@echo "Cleaned successfully"

.PHONY: all clean tests

run: all
	./tools/run.sh $(radius)

test: tests
	@if [ -z "$(name)" ]; then \
		./tools/run_all_tests.sh $(radius); \
	else \
		./tools/run_test.sh $(name) $(radius); \
	fi

.PHONY: run test

help:
	@echo "Available commands:"
	@echo "  make                             - Compile the main application"
	@echo "  make run                         - Compile and run the WFC (default)"
	@echo "  make tests                       - Compile all test binaries"
	@echo "  make test                        - Run all tests and generate images"
	@echo "  make test name=perlin radius=30  - Run a specific test"
	@echo "  make clean                       - Remove binaries and object files"
	@echo "  make help                        - Show this help message"

.PHONY: help

# Simple Makefile for hexagonal map project

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
SRCS_CPP := $(shell find $(SRC_DIR) -name "*.cpp")
SRCS_C   := $(shell find $(SRC_DIR) -name "*.c")

OBJS := $(SRCS_CPP:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
        $(SRCS_C:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

DEPS := $(OBJS:.o=.d)

# Main Target
TARGET = $(BIN_DIR)/wfc

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)
	@echo "Built successfully: $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
	@echo "Cleaned successfully"

.PHONY: all clean

# Simple Makefile for hexagonal map project

CXX      := g++
CC       := gcc
CXXFLAGS := -std=c++17 -O0 -Wall
LIBS     := -lglfw -lGL -ldl -lpthread -lX11 -lXrandr -lXi

# Paths
ROOT_DIR := WFC_Core
SRC_DIR  := $(ROOT_DIR)/src
INC_DIR  := $(ROOT_DIR)/include
BIN_DIR  := bin
OBJ_DIR  := obj
INCLUDES := -I$(INC_DIR)

# Source Files
SRCS := $(SRC_DIR)/main.cpp \
        $(SRC_DIR)/engine/wfc.cpp \
        $(SRC_DIR)/engine/hex_to_pixels.cpp \
        $(SRC_DIR)/models/cell.cpp \
        $(SRC_DIR)/models/hex.cpp

GLAD_SRC := $(SRC_DIR)/external/glad.c

OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
GLAD_OBJ := $(OBJ_DIR)/external/glad.o

# Main Target
TARGET = $(BIN_DIR)/wfc

all: $(TARGET)

$(TARGET): $(OBJS) $(GLAD_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)
	@echo "Built successfully: $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(GLAD_OBJ): $(GLAD_SRC)
	@mkdir -p $(dir $@)
	$(CC) -O2 $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
	@echo "Cleaned successfully"

.PHONY: all clean

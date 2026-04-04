# Simple Makefile for hexagonal map project

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Iinclude

SRC_DIR = src
BIN_DIR = bin

# Target executable
TARGET = bin/hexmap

# Source files
SRC = hex.cpp hexagonal_map.cpp wave_function_collapse.cpp

wfc: $(SRC_DIR)/wave_function_collapse.cpp $(SRC_DIR)/hex.cpp $(SRC_DIR)/cell.cpp
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/wfc $^

clean:
	rm -rf $(BIN_DIR)

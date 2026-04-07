# Simple Makefile for hexagonal map project

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Iinclude

# Linker Libs - Order matters!
# -lglfw: window management
# -lGL: opengl core
# -ldl: Necessário para o GLAD carregar funções em tempo de execução
# -ldl: required to GLAD load funcs in execution time
# -lpthread: system threads
LIBS = -lglfw -lGL -ldl -lpthread -lX11 -lXrandr -lXi

# Paths
SRC_DIR = src
BIN_DIR = bin

# Source Files
SRC = $(SRC_DIR)/wave_function_collapse.cpp \
      $(SRC_DIR)/hex.cpp \
      $(SRC_DIR)/cell.cpp \
      $(SRC_DIR)/glad.c

# Main Target
TARGET = $(BIN_DIR)/wfc

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -rf $(BIN_DIR)

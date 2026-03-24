# Simple Makefile for hexagonal map project

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

# Target executable
TARGET = hexmap

# Source files
SRC = hex.cpp hexagonal_map.cpp

# Build rule
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean rule
clean:
	rm -f $(TARGET) *.o
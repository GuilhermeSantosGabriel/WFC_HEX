# Simple Makefile for hexagonal map project

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

# Target executable
TARGET = hexmap

# Source files
SRC = hex.cpp hexagonal_map.cpp wave_function_collapse.cpp

wfc: wave_function_collapse.cpp hex.cpp cell.cpp
	$(CXX) $(CXXFLAGS) -o wfc wave_function_collapse.cpp hex.cpp cell.cpp

# Build rule
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean rule
clean:
	rm -f $(TARGET) *.o


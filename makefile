CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -municode -O2
SRC = src
BIN = bin

all: $(BIN)/fcopy $(BIN)/fpaste

$(BIN)/fcopy: $(SRC)/fcopy.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(BIN)/fpaste: $(SRC)/fpaste.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(BIN)/fcopy $(BIN)/fpaste

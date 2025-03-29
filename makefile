CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -municode -O2
SRC = src
BIN = bin
PROGS = fcopy fpaste

all: $(PROGS:%=$(BIN)/%)

$(BIN)/%: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(PROGS:%=$(BIN)/%)

.PHONY: all clean

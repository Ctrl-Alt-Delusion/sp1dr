CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I./src
SRC_DIR := src
BIN := sp1dr

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS := $(SOURCES:.cpp=.o)

.PHONY: all build run clean rebuild debug

all: build

rebuild: clean build run

build: $(BIN)

$(BIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: build
	./$(BIN)

clean:
	rm -f $(BIN) $(OBJECTS)

debug:
	@echo "Sources: $(SOURCES)"
	@echo "Objects: $(OBJECTS)"

# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I./src

ifeq ($(OS),Windows_NT)
	BIN := sp1dr.exe
	RM := del /Q
	FIND := for /R $(SRC_DIR) %f in (*.cpp) do @echo %f
	SHELL := cmd
else
	BIN := sp1dr
	RM := rm -f
	FIND := find $(SRC_DIR) -name '*.cpp'
	SHELL := /bin/bash
endif

SRC_DIR := src
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
ifeq ($(OS),Windows_NT)
	$(BIN)
else
	./$(BIN)
endif

clean:
	$(RM) $(BIN) $(OBJECTS)

debug:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"

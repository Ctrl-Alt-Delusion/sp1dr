CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I./src

SRC_DIR := src
BIN := sp1dr.exe
RM := del /Q /F

ifeq ($(OS),Windows_NT)
	SOURCES := $(shell cmd /C list_cpp.bat $(SRC_DIR))
else
	SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
endif

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
	$(BIN)

clean:
	-$(RM) $(BIN) $(OBJECTS)

debug:
	@echo SOURCES = $(SOURCES)
	@echo OBJECTS = $(OBJECTS)

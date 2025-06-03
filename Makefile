# Cross-Platform Makefile for sp1dr
# Works on Windows, macOS, Linux, FreeBSD, RedStar OS, and other Unix-like systems

# Compiler settings - consistent across platforms
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I./src

# Project structure
SRC_DIR := src
BIN_NAME := sp1dr

# Platform detection using multiple methods for maximum compatibility
# This approach works even when OS environment variable isn't set
UNAME_S := $(shell uname -s 2>/dev/null || echo "Unknown")
UNAME_M := $(shell uname -m 2>/dev/null || echo "Unknown")

# Initialize platform-specific variables
PLATFORM := unknown
EXE_EXT := 
RM_CMD := rm -f
RM_FLAGS := 
PATH_SEP := /
SHELL_NULL := /dev/null

# Windows detection (handles multiple Windows environments)
# Check for common Windows indicators in multiple ways
ifneq ($(findstring Windows,$(OS)),)
    PLATFORM := windows
else ifneq ($(findstring MINGW,$(UNAME_S)),)
    PLATFORM := windows
else ifneq ($(findstring MSYS,$(UNAME_S)),)
    PLATFORM := windows
else ifneq ($(findstring CYGWIN,$(UNAME_S)),)
    PLATFORM := windows
else ifeq ($(UNAME_S),)
    # If uname fails completely, assume Windows (common in some Windows shells)
    PLATFORM := windows
endif

# Apply platform-specific settings
ifeq ($(PLATFORM),windows)
    EXE_EXT := .exe
    # Use cross-platform delete command that works in both cmd and PowerShell
    RM_CMD := if exist
    RM_FLAGS := del /Q /F
    SHELL_NULL := NUL
    # Windows file discovery - works without external batch files
    SOURCES := $(shell dir /s /b "$(SRC_DIR)\*.cpp" 2>$(SHELL_NULL) | tr '\\' '/')
    # Fallback if dir command fails (e.g., in Git Bash)
    ifeq ($(SOURCES),)
        SOURCES := $(shell find $(SRC_DIR) -name "*.cpp" 2>$(SHELL_NULL) || echo "")
    endif
else
    # Unix-like systems (Linux, macOS, FreeBSD, RedStar OS, etc.)
    SOURCES := $(shell find $(SRC_DIR) -name "*.cpp" 2>$(SHELL_NULL) || echo "")
endif

# Final binary name with platform-appropriate extension
BIN := $(BIN_NAME)$(EXE_EXT)

# Generate object file names from source files
OBJECTS := $(SOURCES:.cpp=.o)

# Phony targets (targets that don't represent files)
.PHONY: all build run clean rebuild debug help platform-info

# Default target
all: build

# Quick rebuild cycle - clean, build, and run
rebuild: clean build run

# Main build target
build: platform-info $(BIN)

# Link the final executable
$(BIN): $(OBJECTS)
	@echo "Linking executable: $@"
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build successful! Executable: $@"

# Compile source files to object files
%.o: %.cpp
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the built executable
run: build
	@echo "Running $(BIN)..."
	@./$(BIN)

# Cross-platform clean command
clean:
	@echo "Cleaning build artifacts..."
ifeq ($(PLATFORM),windows)
	@for %%f in ($(BIN) $(OBJECTS)) do @if exist "%%f" $(RM_FLAGS) "%%f"
else
	@$(RM_CMD) $(BIN) $(OBJECTS) 2>$(SHELL_NULL) || true
endif
	@echo "Clean completed."

# Debug information - shows detected platform and file lists
debug: platform-info
	@echo "=== Build Configuration Debug ==="
	@echo "CXX = $(CXX)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "SRC_DIR = $(SRC_DIR)"
	@echo "BIN = $(BIN)"
	@echo "SOURCES = $(SOURCES)"
	@echo "OBJECTS = $(OBJECTS)"
	@echo "RM_CMD = $(RM_CMD)"
	@echo "================================="

# Display platform information
platform-info:
	@echo "=== Platform Detection ==="
	@echo "Detected Platform: $(PLATFORM)"
	@echo "System: $(UNAME_S)"
	@echo "Architecture: $(UNAME_M)"
	@echo "Executable Extension: $(EXE_EXT)"
	@echo "=========================="

# Help target - explains available commands
help:
	@echo "Available targets:"
	@echo "  all        - Build the project (default)"
	@echo "  build      - Compile and link the executable"
	@echo "  run        - Build and run the executable"
	@echo "  clean      - Remove build artifacts"
	@echo "  rebuild    - Clean, build, and run"
	@echo "  debug      - Show build configuration"
	@echo "  platform-info - Show detected platform"
	@echo "  help       - Show this help message"

# Automatic dependency generation for header files
# This ensures that when you modify a header file, dependent source files recompile
-include $(OBJECTS:.o=.d)

%.d: %.cpp
	@$(CXX) $(CXXFLAGS) -MM -MT $(@:.d=.o) $< > $@
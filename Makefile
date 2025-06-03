# Cross-platform Makefile for C++ project
# Automatically detects platform and uses appropriate commands

# Detect platform
ifeq ($(OS),Windows_NT)
    PLATFORM := windows
    DETECTED_OS := Windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        PLATFORM := linux
        DETECTED_OS := Linux
    endif
    ifeq ($(UNAME_S),Darwin)
        PLATFORM := macos
        DETECTED_OS := macOS
    endif
endif

# Set null device based on platform
ifeq ($(PLATFORM),windows)
    NULL_DEVICE := nul
    PATH_SEP := \\
    EXE_EXT := .exe
else
    NULL_DEVICE := /dev/null
    PATH_SEP := /
    EXE_EXT :=
endif

# Project configuration
PROJECT_NAME := sp1dr
SRC_DIR := src
BUILD_DIR := build
TARGET := $(PROJECT_NAME)$(EXE_EXT)

# Compiler settings
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
LDFLAGS :=

# Platform-specific source file discovery
ifeq ($(PLATFORM),windows)
    # Try multiple methods for Windows compatibility
    # Method 1: PowerShell (most reliable on modern Windows)
    SOURCES := $(shell powershell -Command "Get-ChildItem -Recurse -Filter '*.cpp' -Path '$(SRC_DIR)' | ForEach-Object { $$_.FullName -replace '\\\\', '/' }" 2>$(NULL_DEVICE))
    
    # Method 2: Git Bash find (if PowerShell fails)
    ifeq ($(SOURCES),)
        SOURCES := $(shell find $(SRC_DIR) -name "*.cpp" 2>$(NULL_DEVICE))
    endif
    
    # Method 3: Windows FOR command (fallback)
    ifeq ($(SOURCES),)
        SOURCES := $(shell for /R $(SRC_DIR) %%f in (*.cpp) do @echo %%f 2>$(NULL_DEVICE))
        SOURCES := $(subst \,/,$(SOURCES))
    endif
    
    # Method 4: Manual specification (ultimate fallback)
    ifeq ($(SOURCES),)
        SOURCES := src/main.cpp src/core/core.cpp src/core/renderer.cpp src/core/screen.cpp
    endif
else
    # Unix-like systems (Linux, macOS)
    SOURCES := $(shell find $(SRC_DIR) -name "*.cpp" 2>$(NULL_DEVICE))
endif

# Generate object file names
OBJECTS := $(SOURCES:.cpp=.o)

# Create build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR) 2>$(NULL_DEVICE))

# Default target
.PHONY: all clean info help

all: info $(TARGET)

# Display build information
info:
	@echo "Building $(PROJECT_NAME) for $(DETECTED_OS)"
	@echo "Compiler: $(CXX)"
	@echo "Sources found: $(words $(SOURCES)) files"
	@echo "Sources: $(SOURCES)"
	@echo ""

# Link the executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
ifeq ($(PLATFORM),windows)
	@echo "Cleaning build artifacts (Windows)..."
	@if exist $(TARGET) del /Q $(TARGET) 2>$(NULL_DEVICE)
	@for /R $(SRC_DIR) %%f in (*.o) do @if exist "%%f" del /Q "%%f" 2>$(NULL_DEVICE)
else
	@echo "Cleaning build artifacts (Unix)..."
	@rm -f $(TARGET) $(OBJECTS) 2>$(NULL_DEVICE)
endif
	@echo "Clean complete."

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build the project (default)"
	@echo "  clean   - Remove build artifacts"
	@echo "  info    - Show build information"
	@echo "  help    - Show this help message"
	@echo ""
	@echo "Platform detected: $(DETECTED_OS)"
	@echo "Target executable: $(TARGET)"

# Debug target to show variables
debug:
	@echo "Platform: $(PLATFORM)"
	@echo "OS: $(DETECTED_OS)"
	@echo "Sources: $(SOURCES)"
	@echo "Objects: $(OBJECTS)"
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
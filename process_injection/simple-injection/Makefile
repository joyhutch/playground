# Makefile for shellcode testing program
# Targets: debug and release builds

# TODO -- auto create bin/ directory if it doesn't exist

# Compiler and flags
CXX = g++
TARGET = bin/inject_me

# Debug build flags
DEBUG_FLAGS = -g -O0
DEBUG_LIBS = 

# Release build flags  
RELEASE_FLAGS = -O2 -DNDEBUG -s
RELEASE_LIBS = 

# Local Injection Target
LOCAL_SOURCE = inject-local-process.cpp
LOCAL_TARGET = $(TARGET)_local.exe
LOCAL_DEBUG_TARGET = $(TARGET)_local_debug.exe

# Remote Injection Target
REMOTE_SOURCE = inject-remote.cpp
REMOTE_TARGET = $(TARGET)_remote.exe
REMOTE_DEBUG_TARGET = $(TARGET)_remote_debug.exe

# Windows-specific flags
WINDOWS_FLAGS = -static-libgcc -static-libstdc++

# Default target
all: debug release

# Debug build
local-debug: $(LOCAL_DEBUG_TARGET)

$(LOCAL_DEBUG_TARGET): $(LOCAL_SOURCE)
	@echo "Building local debug version..."
	$(CXX) $(DEBUG_FLAGS) $(WINDOWS_FLAGS) -o $(LOCAL_DEBUG_TARGET) $(LOCAL_SOURCE) $(DEBUG_LIBS)
	@echo "Local debug build complete: $(LOCAL_DEBUG_TARGET)"


# Release build
local-release: $(LOCAL_TARGET)

$(LOCAL_TARGET): $(LOCAL_SOURCE)
	@echo "Building local release version..."
	$(CXX) $(RELEASE_FLAGS) $(WINDOWS_FLAGS) -o $(LOCAL_TARGET) $(LOCAL_SOURCE) $(RELEASE_LIBS)
	@echo "Local release build complete: $(LOCAL_TARGET)"

# Debug build
remote-debug: $(REMOTE_DEBUG_TARGET)

$(REMOTE_DEBUG_TARGET): $(REMOTE_SOURCE)
	@echo "Building remote debug version..."
	$(CXX) $(DEBUG_FLAGS) $(WINDOWS_FLAGS) -o $(REMOTE_DEBUG_TARGET) $(REMOTE_SOURCE) $(DEBUG_LIBS)
	@echo "Debug build complete: $(REMOTE_DEBUG_TARGET)"

# Release build
remote-release: $(REMOTE_TARGET)

$(REMOTE_TARGET): $(REMOTE_SOURCE)
	@echo "Building release version..."
	$(CXX) $(RELEASE_FLAGS) $(WINDOWS_FLAGS) -o $(REMOTE_TARGET) $(REMOTE_SOURCE) $(RELEASE_LIBS)
	@echo "Release build complete: $(REMOTE_TARGET)"


# Clean up build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf bin/
	@echo "Clean complete"

# Show help
help:
	@echo "Available targets:"
	@echo "  all           - Build both debug and release versions"
	@echo "  local-debug    - Build debug version with symbols"
	@echo "  local-release  - Build optimized release version"
	@echo "  remote-debug   - Build remote debug version with symbols"
	@echo "  remote-release - Build optimized remote release version"
	@echo "  clean         - Remove build artifacts"
	@echo "  help          - Show this help message"

# Mark phony targets
.PHONY: all local-debug local-release remote-debug remote-release clean help
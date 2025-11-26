# Project settings
PROJECT_NAME = main.exe
BUILD_DIR = build
SRC_DIR = src
OBJ_DIR = $(BUILD_DIR)/obj

# Compiler settings
CXX = clang++
CC = clang
CXXFLAGS = -std=c++26 -Wall -Wextra -g
CFLAGS = -Wall -Wextra -g

# Detect OS
ifeq ($(OS),Windows_NT)
	DETECTED_OS := Windows
else
	DETECTED_OS := $(shell uname -s)
endif

# Common includes
INCLUDES = -I./include 

# Find all .cpp files using wildcard (supports up to 3 levels deep)
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) \
          $(wildcard $(SRC_DIR)/*/*.cpp) \
          $(wildcard $(SRC_DIR)/*/*/*.cpp) \
          $(wildcard include/*.cpp) \
          $(wildcard include/*/*.cpp) \
          $(wildcard include/*/*/*.cpp)

# GLAD source
GLAD_SRC = vendor/glad/src/glad.c
GLAD_OBJ = $(OBJ_DIR)/glad.o

# OS-specific settings
ifeq ($(DETECTED_OS),Windows)
	# Windows-specific settings
	PROJECT_NAME = HibiBrowser.exe
	LIBDIRS = 
	LIBS = -lopengl32 -lgdi32 -luser32 -lkernel32 -lshell32 -lmsvcrt -lucrt
	LDFLAGS = -Wl,/NODEFAULTLIB:libcmt
	MKDIR = @if not exist "$(1)" mkdir "$(1)"
	RM = @if exist "$(1)" powershell -Command "Get-ChildItem -Path '$(1)' -Exclude 'gamedata' | Remove-Item -Recurse -Force"
else
	$(error Unsupported operating system: $(DETECTED_OS))
endif

# Convert sources to objects
define src_to_obj
$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(subst /,_,$(subst \,_,$(1))))
endef

OBJECTS = $(foreach src,$(SOURCES),$(call src_to_obj,$(notdir $(src))))

# Target executable
TARGET = $(BUILD_DIR)/$(PROJECT_NAME)

# Default target
all:
	@echo Building for $(DETECTED_OS)...
	@$(MAKE) $(TARGET)

# Create target executable
$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBDIRS) $(LIBS)
	@echo Build complete: $(TARGET)

# Create specific rules for each source file found
define create_obj_rule
$(OBJ_DIR)/$(subst /,_,$(subst \,_,$(notdir $(1:.cpp=.o)))): $(1) | $(OBJ_DIR)
	@echo Compiling $$<...
	$(call MKDIR,$$(dir $$@))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $$< -o $$@
endef

$(foreach src,$(SOURCES),$(eval $(call create_obj_rule,$(src))))

# Create directories
$(BUILD_DIR):
	$(call MKDIR,$(BUILD_DIR))

$(OBJ_DIR):
	$(call MKDIR,$(OBJ_DIR))

# Clean build files
clean:
	$(call RM,$(BUILD_DIR))
	@echo Clean complete

# Run the executable
run: $(TARGET)
	$(TARGET)

# Debug build
debug: CXXFLAGS += -DDEBUG -O0
debug: all

# Release build
release: CXXFLAGS += -DNDEBUG -O3
release: all

# Print variables for debugging
print-%:
	@echo $* = $($*)

print-os:
	@echo Detected OS: $(DETECTED_OS)

.PHONY: all clean run debug release print-% print-os

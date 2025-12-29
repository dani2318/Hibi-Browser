# Project settings
PROJECT_NAME = HibiBrowser.exe
BUILD_DIR = build
SRC_DIR = src
OBJ_DIR = $(BUILD_DIR)/obj
DLL_DIR = $(BUILD_DIR)/dll
INCLUDE_DIR = include/core

# Compiler settings
CXX = clang++
CC = clang
CXXFLAGS = -std=c++26 -Wall -Wextra -g
CFLAGS = -Wall -Wextra -g
DLL_CXXFLAGS = -std=c++26 -Wall -Wextra -g -DBUILD_DLL

# Detect OS
ifeq ($(OS),Windows_NT)
	DETECTED_OS := Windows
else
	DETECTED_OS := $(shell uname -s)
endif

# Common includes
INCLUDES = -I./include 

# Define modules in dependency order (dependencies first)
MODULES = fonts render parser html net

# OS-specific settings
ifeq ($(DETECTED_OS),Windows)
	DLL_EXT = .dll
	LIBS = -lopengl32 -lgdi32 -luser32 -lkernel32 -lshell32 -lmsvcrt -lucrt
	LDFLAGS = -Wl,/NODEFAULTLIB:libcmt
	MKDIR = @if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
	RM = @if exist "$(subst /,\,$(1))" powershell -Command "Get-ChildItem -Path '$(1)' -Exclude 'gamedata' | Remove-Item -Recurse -Force"
else
	$(error Unsupported operating system: $(DETECTED_OS))
endif

# Find sources for each module
define get_module_sources
$(wildcard $(INCLUDE_DIR)/$(1)/*.cpp)
endef

# Get object files for a module
define get_module_objects
$(foreach src,$(call get_module_sources,$(1)),$(call src_to_obj,$(src)))
endef

# Find main executable sources (everything not in module folders)
MAIN_SOURCES = $(wildcard $(SRC_DIR)/*.cpp) \
               $(wildcard $(INCLUDE_DIR)/*.cpp)

# Generate DLL names
DLLS = $(foreach mod,$(MODULES),$(DLL_DIR)/$(mod)$(DLL_EXT))

# Convert sources to objects for main executable
define src_to_obj
$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(subst /,_,$(subst \,_,$(notdir $(1)))))
endef

MAIN_OBJECTS = $(foreach src,$(MAIN_SOURCES),$(call src_to_obj,$(src)))

# Get all module object files
FONTS_OBJS = $(call get_module_objects,fonts)
RENDER_OBJS = $(call get_module_objects,render)
PARSER_OBJS = $(call get_module_objects,parser)
HTML_OBJS = $(call get_module_objects,html)
NET_OBJS = $(call get_module_objects,net)

# All object files for main executable (includes all modules)
ALL_OBJS = $(MAIN_OBJECTS) $(FONTS_OBJS) $(RENDER_OBJS) $(PARSER_OBJS) $(HTML_OBJS) $(NET_OBJS)

# Target executable
TARGET = $(BUILD_DIR)/$(PROJECT_NAME)

# Default target
all:
	@echo Building for $(DETECTED_OS)...
	@$(MAKE) $(DLLS)
	@$(MAKE) $(TARGET)

# Create target executable - include all object files
$(TARGET): $(ALL_OBJS) $(DLLS) | $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $(ALL_OBJS) -o $@ $(LIBS)
	@echo Copying DLLs to executable directory...
	@copy /Y "$(subst /,\,$(DLL_DIR)\*.dll)" "$(subst /,\,$(BUILD_DIR))" >nul 2>&1
	@echo Build complete: $(TARGET)

# Specific DLL rules with dependencies - link object files from dependencies
$(DLL_DIR)/fonts$(DLL_EXT): $(FONTS_OBJS) | $(DLL_DIR)
	@echo Creating DLL: fonts$(DLL_EXT)
	$(CXX) -shared -Wl,/NODEFAULTLIB:libcmt $(FONTS_OBJS) -o $@ $(LIBS)
	@echo DLL complete: $@

$(DLL_DIR)/render$(DLL_EXT): $(RENDER_OBJS) $(FONTS_OBJS) | $(DLL_DIR)
	@echo Creating DLL: render$(DLL_EXT)
	$(CXX) -shared -Wl,/NODEFAULTLIB:libcmt $(RENDER_OBJS) $(FONTS_OBJS) -o $@ $(LIBS)
	@echo DLL complete: $@

$(DLL_DIR)/parser$(DLL_EXT): $(PARSER_OBJS) $(RENDER_OBJS) $(FONTS_OBJS) | $(DLL_DIR)
	@echo Creating DLL: parser$(DLL_EXT)
	$(CXX) -shared -Wl,/NODEFAULTLIB:libcmt $(PARSER_OBJS) $(RENDER_OBJS) $(FONTS_OBJS) -o $@ $(LIBS)
	@echo DLL complete: $@

$(DLL_DIR)/html$(DLL_EXT): $(HTML_OBJS) $(PARSER_OBJS) $(RENDER_OBJS) $(FONTS_OBJS) | $(DLL_DIR)
	@echo Creating DLL: html$(DLL_EXT)
	$(CXX) -shared -Wl,/NODEFAULTLIB:libcmt $(HTML_OBJS) $(PARSER_OBJS) $(RENDER_OBJS) $(FONTS_OBJS) -o $@ $(LIBS)
	@echo DLL complete: $@

$(DLL_DIR)/net$(DLL_EXT): $(NET_OBJS) | $(DLL_DIR)
	@echo Creating DLL: net$(DLL_EXT)
	$(CXX) -shared -Wl,/NODEFAULTLIB:libcmt $(NET_OBJS) -o $@ $(LIBS)
	@echo DLL complete: $@

# Create object file compilation rule
define create_obj_rule
$(OBJ_DIR)/$(subst /,_,$(subst \,_,$(notdir $(1:.cpp=.o)))): $(1) | $(OBJ_DIR)
	@echo Compiling $$<...
	$(call MKDIR,$$(dir $$@))
	$(CXX) $(DLL_CXXFLAGS) $(INCLUDES) -c $$< -o $$@
endef

# Create rules for all sources (both main and modules)
$(foreach mod,$(MODULES),$(foreach src,$(call get_module_sources,$(mod)),$(eval $(call create_obj_rule,$(src)))))
$(foreach src,$(MAIN_SOURCES),$(eval $(call create_obj_rule,$(src))))

# Create directories
$(BUILD_DIR):
	$(call MKDIR,$(BUILD_DIR))

$(OBJ_DIR):
	$(call MKDIR,$(OBJ_DIR))

$(DLL_DIR):
	$(call MKDIR,$(DLL_DIR))

# Clean build files
clean:
	$(call RM,$(BUILD_DIR))
	@echo Clean complete

# Run the executable
run: $(TARGET)
	$(TARGET)

# Debug build
debug: CXXFLAGS += -DDEBUG -O0
debug: DLL_CXXFLAGS += -DDEBUG -O0
debug: all

# Release build
release: CXXFLAGS += -DNDEBUG -O3
release: DLL_CXXFLAGS += -DNDEBUG -O3
release: all

# Build only DLLs
dlls: $(DLLS)

# Print variables for debugging
print-%:
	@echo $* = $($*)

print-os:
	@echo Detected OS: $(DETECTED_OS)

print-modules:
	@echo Modules: $(MODULES)
	@echo DLLs: $(DLLS)

.PHONY: all clean run debug release dlls print-% print-os print-modules
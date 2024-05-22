################################################################################
#### General configurations
################################################################################

# Executable name
EXEC = equinox

# Build and assets directories
BUILD_DIR = build
ASSETS_DIR = assets

SRC_DIR = src
INCLUDE_DIR = include
ECS_MODULE = ecs

# Sources
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) $(wildcard $(INCLUDE_DIR)/$(ECS_MODULE)/$(SRC_DIR)/*.cpp) $(wildcard $(INCLUDE_DIR)/$(ECS_MODULE)/$(SRC_DIR)/*/*.cpp)

# Includes
INCLUDES := -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/headers -I$(INCLUDE_DIR)/templates -I$(INCLUDE_DIR)/$(ECS_MODULE)/$(INCLUDE_DIR)/headers -I$(INCLUDE_DIR)/$(ECS_MODULE)/$(INCLUDE_DIR)/templates -I$(INCLUDE_DIR)/sdl2

# C preprocessor settings
CPPFLAGS = $(INCLUDES) -MMD -MP

# C++ compiler settings
CXX = g++
CXXFLAGS = -std=c++17
WARNINGS = -Wall -Wextra

# Linker flags
LDFLAGS =

# Libraries to link
# LDLIBS = -l"SDL2" -l"SDL2_image" -l"SDL2_ttf" -l"SDL2_mixer"
LDLIBS = -l"SDL2" -l"SDL2_image"

# Target OS detection
ifeq ($(OS),Windows_NT)   # OS is a preexisting environment variable on Windows
	OS = windows
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Darwin)
		OS = macos
	else ifeq ($(UNAME),Linux)
		OS = linux
	else
    	$(error Unsupported OS)
	endif
endif

################################################################################
#### Platform-specific configurations
################################################################################

ifeq ($(OS),windows)   # Windows 32-bit and 64-bit common settings
	INCLUDES +=
	LDFLAGS += -static-libgcc -static-libstdc++
	# LDLIBS := -l"mingw32" -l"SDL2main" $(LDLIBS) -l"zlib"
	LDLIBS := -l"mingw32" -l"SDL2main" $(LDLIBS)

	LIB_PATH = -L"lib"
	EXEC := $(EXEC).exe
	RM = del /Q

	ifeq ($(win32),1)
		# Windows 32-bit settings
		INCLUDES +=
		LDFLAGS +=
		LDLIBS +=

		CXXFLAGS += -m32
	else
		# Windows 64-bit settings
		INCLUDES +=
		LDFLAGS +=
		LDLIBS +=

		CXXFLAGS += -m64
	endif

else ifeq ($(OS),macos)   # macOS-specific settings
	INCLUDES +=
	LDFLAGS +=
	# LDLIBS += -l"z"

else ifeq ($(OS),linux)   # Linux-specific settings
	INCLUDES +=
	LDFLAGS +=
	# LDLIBS += -l"z"

	LIB_PATH = -L/usr/lib
	RM = rm -f
endif

OUTPUT = $(BUILD_DIR)/$(EXEC)

################################################################################
#### Targets
################################################################################

.PHONY: all
all: $(OUTPUT)

$(OUTPUT): $(SRCS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNINGS) $(LIB_PATH) -o $@ $^ $(LDLIBS)

.PHONY: run
run:
	./$(OUTPUT)

.PHONY: rund
rund:
	gdb -ex run ./$(OUTPUT)

.PHONY: clean
clean:
	@echo Cleaning $(BUILD_DIR) directory
	$(RM) $(OUTPUT)
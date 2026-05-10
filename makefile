CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude
ifeq ($(OS),Windows_NT)
	CXXGFLAGS := -lraylib -lopengl32 -lgdi32 -lwinmm
	MKDIR_P = if not exist "$(subst /,\,$1)" mkdir "$(subst /,\,$1)"
	RM_OUTPUTS = if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)" & if exist "$(BIN_DIR)" del /q "$(BIN_DIR)\*.exe"
else
	CXXGFLAGS := -lraylib -lm -ldl -lpthread -lGL -lX11
	MKDIR_P = mkdir -p $1
	RM_OUTPUTS = rm -rf $(BUILD_DIR) $(BIN_DIR)/*.exe
endif

SRC_DIR := src
BIN_DIR := bin
INCLUDE_DIR := include
BUILD_DIR := build

BACKEND_UTIL_SRCS := $(filter-out $(SRC_DIR)/utils/GuiHelper.cpp,$(wildcard $(SRC_DIR)/utils/*.cpp))

BACKEND_SRCS := $(wildcard \
	$(SRC_DIR)/board/*.cpp \
	$(SRC_DIR)/solver/Algorithm/*.cpp \
	$(SRC_DIR)/solver/Heuristic/*.cpp) \
	$(BACKEND_UTIL_SRCS)

GUI_VIEW_SRCS := $(wildcard \
	$(SRC_DIR)/library/*.cpp \
	$(SRC_DIR)/view/*.cpp \
	$(SRC_DIR)/view/assets/*.cpp \
	$(SRC_DIR)/view/components/*.cpp \
	$(SRC_DIR)/view/scenes/*.cpp) \
	$(SRC_DIR)/utils/GuiHelper.cpp

CLI_SRCS := $(BACKEND_SRCS) $(SRC_DIR)/mainCLI.cpp
GUI_SRCS := $(BACKEND_SRCS) $(GUI_VIEW_SRCS) $(SRC_DIR)/mainGUI.cpp

CLI_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CLI_SRCS))
GUI_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(GUI_SRCS))

CLI := $(BIN_DIR)/mainCLI.exe
GUI := $(BIN_DIR)/mainGUI.exe

all: cli gui
run: cli
	$(CLI)

rung: gui
	$(GUI)

cli: $(CLI)
gui: $(GUI)

$(CLI) : $(CLI_OBJ)
	@$(call MKDIR_P,$(dir $@))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(GUI) : $(GUI_OBJ)
	@$(call MKDIR_P,$(dir $@))
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXGFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(call MKDIR_P,$(dir $@))
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@$(RM_OUTPUTS)

.PHONY: all clean run rung cli gui

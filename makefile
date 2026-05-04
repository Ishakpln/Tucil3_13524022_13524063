CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude

SRC_DIR := src
BIN_DIR := bin
INCLUDE_DIR := include
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp \
	$(SRC_DIR)/board/*.cpp \
	$(SRC_DIR)/solver/Algorithm/*.cpp \
	$(SRC_DIR)/solver/Heuristic/*.cpp \
	$(SRC_DIR)/utils/*.cpp)

OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

CLI := $(BIN_DIR)/mainCLI.exe
GUI := $(BIN_DIR)/mainGUI.exe

all: cli gui
run: cli
	$(CLI)
cli: $(CLI)
gui: $(GUI)

$(CLI) : $(OBJS) $(BUILD_DIR)/mainCLI.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(GUI) : $(OBJS) $(BUILD_DIR)/mainGUI.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)/*.exe

.PHONY: all clean
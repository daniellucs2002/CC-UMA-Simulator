# Specify the source directory and build directory
SRC_DIR := /home/lujunchen/CC-UMA-Simulator
BUILD_DIR := /home/lujunchen/CC-UMA-Simulator/build

# Default target
all: cmake-build

# Target for running cmake
cmake-configure:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake $(SRC_DIR)

# Target for building the project using make after cmake
cmake-build: cmake-configure
	cd $(BUILD_DIR) && make
	cd $(SRC_DIR)

# Target for cleaning the build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all cmake-configure cmake-build clean


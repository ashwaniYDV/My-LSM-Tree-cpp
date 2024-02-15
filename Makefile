CXX = g++
CXXFLAGS = -std=c++20 -Wall -g -Iheaders
LDFLAGS =
BUILD_DIR = build
SRC_DIR = src

# Source files
# SRC = src/main.cpp src/driverMgr.cpp src/riderMgr.cpp src/strategyMgr.cpp src/tripMgr.cpp
SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Object files 
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

# Target executable
TARGET = main
FINAL = $(BUILD_DIR)/$(TARGET)

# Build rule
all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(FINAL) $(OBJ)

# Compile rule
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	make

run:
	$(FINAL)

CXX = g++
CXXFLAGS = -std=c++17 -Wall
RAYLIB_INC = -I"D:/raylib/raylib-5.5_win64_mingw-w64/include"
RAYLIB_LIB = -L"D:/raylib/raylib-5.5_win64_mingw-w64/lib" -lraylib -lopengl32 -lgdi32 -lwinmm

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/galaxy_defenders.exe

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(RAYLIB_LIB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(RAYLIB_INC) -c $< -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR) 2>nul || exit 0

clean:
	rmdir /s /q $(BUILD_DIR) 2>nul || exit 0

.PHONY: all clean
CXX = g++
CXXFLAGS = -std=c++17 -Wall -DSFML_STATIC
INCLUDES = -I"C:/SFML-3.0.2-new/SFML-3.0.2/include"
LIBS = "C:/SFML-3.0.2-new/SFML-3.0.2/lib/libsfml-graphics-s.a" \
       "C:/SFML-3.0.2-new/SFML-3.0.2/lib/libsfml-window-s.a" \
       "C:/SFML-3.0.2-new/SFML-3.0.2/lib/libsfml-system-s.a" \
       -lopengl32 -lwinmm -lgdi32
LDFLAGS = -static -mwindows

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/galaxy_defenders.exe

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LIBS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
CC = g++
CXXFLAGS = -std=c++11 -Wall

SDL2_LIB := libs/SDL2/libs/libSDL2.a
SDL2_IMAGE_LIB := libs/SDL2/libs/libSDL2_image.so
BOX2D_LIB := libs/Box2D/libs/libbox2d.a

LDFLAGS += $(SDL2_LIB)
LDFLAGS += $(SDL2_IMAGE_LIB)
LDFLAGS += $(BOX2D_LIB)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

SRC_DIRS := src src/Engine src/Objects src/Sandbox
OBJ_DIR := obj

INC_DIR := src
INC_FLAGS := $(addprefix -I,$(INC_DIR))

CXXFLAGS += $(INC_FLAGS)

SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

TARGET := VergodtEngine

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: src/Engine/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: src/Objects/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: src/Sandbox/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
CC = g++
CXXFLAGS = -std=c++11 -Wall

program_LIBRARIES := SDL2 SDL2_image box2d

LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

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
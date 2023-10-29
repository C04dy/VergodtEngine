CC = g++
CXXFLAGS = -std=c++11 -Wall -O3

SDL2_LIB := libs/SDL2/libs/libSDL2.a
SDL2_IMAGE_LIB := libs/SDL2/libs/libSDL2_image.so
BOX2D_LIB := libs/Box2D/libs/libbox2d.a

LDFLAGS += $(SDL2_LIB)
LDFLAGS += $(SDL2_IMAGE_LIB)
LDFLAGS += $(BOX2D_LIB)

INC_DIR := src
INC_FLAGS := $(addprefix -I,$(INC_DIR))

CXXFLAGS += $(INC_FLAGS)

SRCS := $(wildcard src/*.cpp) $(wildcard src/Engine/*.cpp) $(wildcard src/Objects/*.cpp) $(wildcard src/Sandbox/*.cpp)

TARGET := VergodtEngine

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
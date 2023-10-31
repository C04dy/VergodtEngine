cmake_minimum_required(VERSION 3.10)

project(VergodtEngine)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")

# Specify the locations of your libraries
link_directories(${CMAKE_SOURCE_DIR}/libs/SDL2/libs)
link_directories(${CMAKE_SOURCE_DIR}/libs/Box2D/libs)

# Include directories
include_directories(src)

# Find source files
file(GLOB_RECURSE SRCS "src/*.cpp" "src/Engine/*.cpp" "src/Objects/*.cpp" "src/Sandbox/*.cpp")

add_executable(${PROJECT_NAME} ${SRCS})

# Link libraries
target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image box2d)

cmake_minimum_required(VERSION 3.27.7)

project(VergodtEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -Wall)

add_library(SDL2 STATIC IMPORTED)
set_property(TARGET SDL2 PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/libSDL2.a)

add_library(SDL2_image STATIC IMPORTED)
set_property(TARGET SDL2_image PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/libSDL2_image.a)

add_library(Box2D STATIC IMPORTED)
set_property(TARGET Box2D PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D/lib/libbox2d.a)

file(GLOB SRCS src/*.cpp src/Engine/*.cpp src/Objects/*.cpp src/Sandbox/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE src "libs/SDL2/include")
target_include_directories(${PROJECT_NAME} PRIVATE src "libs/Box2D/include")

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_image Box2D)
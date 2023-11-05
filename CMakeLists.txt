cmake_minimum_required(VERSION 3.27.7)

project(VergodtEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -Wall)

add_library(SDL2 STATIC IMPORTED)
set_property(TARGET SDL2 PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/libSDL2.a)
set_target_properties(SDL2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/include)

add_library(SDL2_image STATIC IMPORTED)
set_property(TARGET SDL2_image PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/lib/libSDL2_image.a)

add_library(Box2D STATIC IMPORTED)
set_property(TARGET Box2D PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D/lib/libbox2d.a)
set_target_properties(SDL2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D/include)

file(GLOB SRCS src/*.cpp src/Engine/*.cpp src/Objects/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_image Box2D)
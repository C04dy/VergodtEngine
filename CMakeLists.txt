cmake_minimum_required(VERSION 3.27.7)

project(VergodtEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -Wall)

if (WIN32)

add_library(SDL STATIC IMPORTED)
set_property(TARGET SDL PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL/lib/libSDL3.dll.a)
set_target_properties(SDL PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL/include)

add_library(SDL_image STATIC IMPORTED)
set_property(TARGET SDL_image PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL/lib/libSDL3_image.dll.a)

else()

add_library(SDL STATIC IMPORTED)
set_property(TARGET SDL PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL/lib/libSDL3.a)
set_target_properties(SDL PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL/include)

add_library(SDL_image STATIC IMPORTED)
set_property(TARGET SDL_image PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL/lib/libSDL3_image.a)

endif()

add_library(Box2D STATIC IMPORTED)
set_property(TARGET Box2D PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D/lib/libbox2d.a)
set_target_properties(Box2D PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D/include)

file(GLOB SRCS src/*.cpp src/Engine/*.cpp src/Engine/VisualScripting/*.cpp src/Objects/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/Math ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/VisualScripting)

target_link_libraries(${PROJECT_NAME} SDL SDL_image Box2D)
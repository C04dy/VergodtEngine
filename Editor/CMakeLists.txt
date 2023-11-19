cmake_minimum_required(VERSION 3.27.7)

project(VergodtEngineEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -Wall)

if (WIN32)

add_library(SDL STATIC IMPORTED)
set_property(TARGET SDL PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../libs/SDL/lib/libSDL3.dll.a)
set_target_properties(SDL PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../libs/SDL/include)

else()

add_library(SDL STATIC IMPORTED)
set_property(TARGET SDL PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../libs/SDL/lib/libSDL3.a)
set_target_properties(SDL PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../libs/SDL/include)

endif()

file(GLOB SRCS src/*.cpp ImGui/Include/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

include_directories(${PROJECT_NAME} src ImGui/Include ../src/util)

target_link_libraries(${PROJECT_NAME} SDL)
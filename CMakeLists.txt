cmake_minimum_required(VERSION 3.27.7)

project(VergodtEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -Wall -static-libgcc -static-libstdc++ -pg -Wno-unused-variable)
add_link_options(-pg)

if (WIN32)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif(WIN32)

set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_INSTALL OFF CACHE BOOL "" FORCE)
set(DISABLE_DYNAMIC ON CACHE BOOL "" FORCE)
set(SDL_STATIC ON)
set(SDLIMAGE_BUILD_SHARED_LIBS OFF)
set(BUILD_SHARED_LIBS OFF)


add_subdirectory(vendor/box2d)
add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendor/SDL_image)
add_subdirectory(Editor)

file(GLOB SRCS src/*.cpp src/Engine/*.cpp src/Objects/*.cpp src/Math/*.cpp src/Util/*.cpp src/Engine/Scripting/VisualScripting/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL_image/include ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL/include 
                    ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/Math ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/Scripting)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static SDL3_image-static box2d)
cmake_minimum_required(VERSION 3.27.7)

project(VergodtEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -Wall -static-libgcc -static-libstdc++ -pg)
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

add_subdirectory(vendor/box2d)
add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendor/SDL_image)
add_subdirectory(vendor/squirrel)
add_subdirectory(vendor/simplesquirrel)

file(GLOB SRCS src/*.cpp src/Engine/*.cpp src/Objects/*.cpp src/Math/*.cpp) #src/Engine/VisualScripting/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL_image/include ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL/include 
                    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/squirrel/include ${CMAKE_CURRENT_SOURCE_DIR}/vendor/squall ${CMAKE_CURRENT_SOURCE_DIR}/vendor/simplesquirrel/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/Math) #${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/VisualScripting)

add_dependencies(simplesquirrel_static squirrel_static)

target_link_libraries(${PROJECT_NAME} SDL3::SDL3 squirrel_static sqstdlib_static simplesquirrel_static SDL3_image-static box2d)
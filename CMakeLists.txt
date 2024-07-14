cmake_minimum_required(VERSION 3.27.7)

project(VergodtEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -Wall)

set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)

add_subdirectory(vendor/box2d)
add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendor/SDL_image)

if (WIN32)

add_library(squirrel STATIC IMPORTED)
set_property(TARGET squirrel PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/vendor/squirrel_windows/libsquirrel_static.a)
set_target_properties(squirrel PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/vendor/squirrel/include)

add_library(sqstdlib STATIC IMPORTED)
set_property(TARGET sqstdlib PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/vendor/squirrel_windows/libsqstdlib_static.a)

else()
add_subdirectory(vendor/squirrel)
endif(WIN32)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})

file(GLOB SRCS src/*.cpp src/Engine/*.cpp src/Engine/VisualScripting/*.cpp src/Objects/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL_image/include ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL/include 
                    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/squirrel/include ${CMAKE_CURRENT_SOURCE_DIR}/vendor/squall 
                    ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/Math) #${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/VisualScripting)

if (WIN32)
target_link_libraries(${PROJECT_NAME} SDL3::SDL3-static squirrel sqstdlib SDL3_image-static box2d)
else()
target_link_libraries(${PROJECT_NAME} SDL3::SDL3-static squirrel SDL3_image-static box2d)
endif(WIN32)
cmake_minimum_required(VERSION 3.27.7)

project(VergodtEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -Wall)

add_subdirectory(vendor/box2d)
add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendor/SDL_image)
add_subdirectory(vendor/squirrel)

file(GLOB SRCS src/*.cpp src/Engine/*.cpp src/Engine/VisualScripting/*.cpp src/Objects/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL_image/include ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL/include 
                    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/squirrel/include ${CMAKE_CURRENT_SOURCE_DIR}/vendor/squall 
                    ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/Math) #${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/VisualScripting)

target_link_libraries(${PROJECT_NAME} SDL3::SDL3 squirrel sqstdlib SDL3_image-static box2d)
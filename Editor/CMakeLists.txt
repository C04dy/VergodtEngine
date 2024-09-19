project(VergodtEngineEditor)

file(GLOB SRCS ./*.cpp src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui/misc/cpp/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui/backends/imgui_impl_sdl3.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui/backends/imgui_impl_sdlrenderer3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui/backends ${CMAKE_CURRENT_SOURCE_DIR}/../src/Util/StringFunctions.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui/misc/cpp ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/SDL_image/src
                    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui/backends ${CMAKE_CURRENT_SOURCE_DIR}/../src/Util ${CMAKE_CURRENT_SOURCE_DIR}/src
                    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/nativefiledialog-extended/src/include)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(BUILD_TESTING OFF)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../vendor/nativefiledialog-extended ${CMAKE_BINARY_DIR}/vendor/nativefiledialog-extended)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../vendor/tiny-process-library ${CMAKE_BINARY_DIR}/vendor/tiny-process-library)

target_link_libraries(${PROJECT_NAME} SDL3::SDL3-static SDL3_image-static nfd tiny-process-library)
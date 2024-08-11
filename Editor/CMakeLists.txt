project(VergodtEngineEditor)

file(GLOB SRCS ./*.cpp src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui/backends/imgui_impl_sdl3.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui/backends/imgui_impl_sdlrenderer3.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imgui/backends ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} SDL3::SDL3)
cmake_minimum_required(VERSION 3.27.7)

project(VergodtEngineModule)

add_subdirectory(pybind11)

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_SOURCE_DIR}/../src/Math)

pybind11_add_module(VergodtEngine Module.cpp)
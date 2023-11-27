cmake_minimum_required(VERSION 3.27.7)

project(PythonModule)

add_subdirectory(pybind11)

pybind11_add_module(test Module.cpp)
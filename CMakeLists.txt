
cmake_minimum_required(VERSION 3.20)

project(Hexgon)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Engine
add_subdirectory(Hexgon)

# Sandbox
add_subdirectory(Sandbox)

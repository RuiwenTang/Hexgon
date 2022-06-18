cmake_minimum_required(VERSION 3.14.0)

project(Hexgon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)



# engine
add_subdirectory(Engine)

#sand-box
add_subdirectory(SandBox)

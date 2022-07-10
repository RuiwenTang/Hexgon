cmake_minimum_required(VERSION 3.14.0)

project(Hexgon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if (WIN32)
  set(BUILD_SHARED_LIBS TRUE)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

# spdlog
add_subdirectory(third_party/spdlog)

# glfw
add_subdirectory(third_party/glfw)

# glm
add_subdirectory(third_party/glm)

# engine
add_subdirectory(Engine)

#sand-box
add_subdirectory(SandBox)

# source group for Xcode and Visual Studio
get_target_property(HEXGON_SRC Hexgon SOURCES)

source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${HEXGON_SRC})

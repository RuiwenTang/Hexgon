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

# SPRIV-Reflect
set(SPIRV_REFLECT_EXECUTABLE OFF CACHE INTERNAL "" FORCE)
set(SPIRV_REFLECT_EXAMPLES OFF CACHE INTERNAL "" FORCE)
set(SPIRV_REFLECT_STATIC_LIB ON CACHE INTERNAL "" FORCE)
add_subdirectory(third_party/SPIRV-Reflect)

# imgui
add_subdirectory(third_party/imgui)

# engine
add_subdirectory(Engine)

#sand-box
add_subdirectory(SandBox)

# source group for Xcode and Visual Studio
get_target_property(HEXGON_SRC Hexgon SOURCES)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEXGON_SRC})

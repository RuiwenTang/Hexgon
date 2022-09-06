
include(${PROJECT_SOURCE_DIR}/CMake/ShaderUtil.cmake)

add_library(SandBoxMaterial STATIC
  ${CMAKE_CURRENT_LIST_DIR}/ColorMaterial.cc
  ${CMAKE_CURRENT_LIST_DIR}/ColorMaterial.hpp
)

add_target_shader(SandBoxMaterial material_shaders SHADERS
  ${CMAKE_CURRENT_LIST_DIR}/color_material.frag
  ${CMAKE_CURRENT_LIST_DIR}/color_material.vert
)

target_include_directories(SandBoxMaterial PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(SandBoxMaterial Hexgon)


include(${PROJECT_SOURCE_DIR}/CMake/ShaderUtil.cmake)

add_executable(HelloTriangle
  ${CMAKE_CURRENT_LIST_DIR}/hello_triangle.cc
)

add_target_shader(HelloTriangle
  hello_triangle_glsl
  SHADERS
  ${CMAKE_CURRENT_LIST_DIR}/Shaders/hello_triangle.vert
  ${CMAKE_CURRENT_LIST_DIR}/Shaders/hello_triangle.frag
  ${CMAKE_CURRENT_LIST_DIR}/Shaders/hello_ubo_triangle.vert
  ${CMAKE_CURRENT_LIST_DIR}/Shaders/hello_ubo_triangle.frag
)

target_link_libraries(HelloTriangle Hexgon)

add_executable(HelloTexture
  ${CMAKE_CURRENT_LIST_DIR}/hello_texture.cc
)

add_definitions(-DRESOURCES_DIR="${CMAKE_CURRENT_LIST_DIR}/Resources")

add_target_shader(HelloTexture
  hello_texture_glsl
  SHADERS
  ${CMAKE_CURRENT_LIST_DIR}/Shaders/hello_texture.vert
  ${CMAKE_CURRENT_LIST_DIR}/Shaders/hello_texture.frag
)

target_link_libraries(HelloTexture Hexgon)

add_executable(HelloImgui
  ${CMAKE_CURRENT_LIST_DIR}/hello_imgui.cc
)

target_link_libraries(HelloImgui Hexgon)

add_executable(HelloMultiLayer
  ${CMAKE_CURRENT_LIST_DIR}/hello_multi_layer.cc
)

add_target_shader(HelloMultiLayer
  hello_layers_glsl
  SHADERS
  ${CMAKE_CURRENT_LIST_DIR}/Shaders/hello_triangle.vert
  ${CMAKE_CURRENT_LIST_DIR}/Shaders/hello_triangle.frag
  ${CMAKE_CURRENT_LIST_DIR}/Shaders/hello_ubo_triangle.vert
  ${CMAKE_CURRENT_LIST_DIR}/Shaders/hello_ubo_triangle.frag
)

target_link_libraries(HelloMultiLayer Hexgon)

add_subdirectory(Material)

add_executable(HelloBox
  ${CMAKE_CURRENT_LIST_DIR}/hello_box.cc
)

target_link_libraries(HelloBox Hexgon SandBoxMaterial)

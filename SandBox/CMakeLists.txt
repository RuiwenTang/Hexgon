
add_subdirectory(Shaders)

add_executable(HelloTriangle
  ${CMAKE_CURRENT_LIST_DIR}/hello_triangle.cc
)

target_link_libraries(HelloTriangle Hexgon ShaderSource)

add_executable(HelloTexture
  ${CMAKE_CURRENT_LIST_DIR}/hello_texture.cc
)

add_definitions(-DRESOURCES_DIR="${CMAKE_CURRENT_LIST_DIR}/Resources")

target_link_libraries(HelloTexture Hexgon ShaderSource)

add_executable(HelloImgui
  ${CMAKE_CURRENT_LIST_DIR}/hello_imgui.cc
)

target_link_libraries(HelloImgui Hexgon ShaderSource imgui)
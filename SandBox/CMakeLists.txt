
add_subdirectory(Shaders)

add_executable(SandBox
  ${CMAKE_CURRENT_LIST_DIR}/main.cc
)

target_link_libraries(SandBox Hexgon)
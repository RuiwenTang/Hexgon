
set(SHADERS
  ${CMAKE_CURRENT_LIST_DIR}/hello_triangle.vert
  ${CMAKE_CURRENT_LIST_DIR}/hello_triangle.frag
  ${CMAKE_CURRENT_LIST_DIR}/hello_ubo_triangle.vert
  ${CMAKE_CURRENT_LIST_DIR}/hello_ubo_triangle.frag
  ${CMAKE_CURRENT_LIST_DIR}/hello_texture.vert
  ${CMAKE_CURRENT_LIST_DIR}/hello_texture.frag
  ${CMAKE_CURRENT_LIST_DIR}/imgui.frag
  ${CMAKE_CURRENT_LIST_DIR}/imgui.vert
)

set(SPV_SHADERS "")

foreach(SHADER ${SHADERS})
  get_filename_component(FILENAME ${SHADER} NAME)
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}.spv
    COMMAND glslc ${SHADER} -o ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}.spv
    DEPENDS ${SHADER}
    COMMENT "Compiling ${FILENAME}"
  )

  set(SPV_SHADERS ${SPV_SHADERS} ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}.spv)

endforeach()

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ShaderSource.hpp
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/Shaders.cmake
  DEPENDS ${SPV_SHADERS}
)

add_library(ShaderSource INTERFACE
  ${CMAKE_CURRENT_BINARY_DIR}/ShaderSource.hpp
)

target_include_directories(ShaderSource INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)


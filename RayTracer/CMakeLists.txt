include(${PROJECT_SOURCE_DIR}/CMake/ShaderUtil.cmake)

add_executable(simple_tracer
	simple_tracer.cc
	Layer/GUILayer.cc
	Layer/GUILayer.hpp
	Layer/RenderLayer.cc
	Layer/RenderLayer.hpp
	Core/Ray.cc
	Core/Ray.hpp
)

add_target_shader(simple_tracer
  texture_glsl
  SHADERS
  ${CMAKE_CURRENT_LIST_DIR}/Shaders/texture.vert
  ${CMAKE_CURRENT_LIST_DIR}/Shaders/texture.frag
)

target_include_directories(
	simple_tracer PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(simple_tracer Hexgon)

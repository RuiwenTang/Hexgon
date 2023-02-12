# add_target_shader: this function add a shader dependency in given target
# this function add a shader dependency in given target and genearte a bin2c header file
# <target> the target name
# <resource_name> the generated header file name without suffix name
# <SHADERS> shader1 shader2... list of shader file
function(add_target_shader target resource_name)
  set(multiValueArgs SHADERS)

  cmake_parse_arguments(PARSED_ARGS "" "" ${multiValueArgs} ${ARGN})

  message("add_target_shaders function call")
  message("target: ${target}")
  message("shaders_header: ${resource_name}")

  foreach(shader ${PARSED_ARGS_SHADERS})
    message("- ${shader}")
    get_filename_component(FILENAME ${shader} NAME)

    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${target}_spvs/${FILENAME}.spv
      COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/${target}_spvs
      COMMAND glslc ${shader} -o ${CMAKE_CURRENT_BINARY_DIR}/${target}_spvs/${FILENAME}.spv
      DEPENDS ${shader}
      COMMENT "Compiling ${FILENAME}"
    )

    list(APPEND SPV_FILES ${CMAKE_CURRENT_BINARY_DIR}/${target}_spvs/${FILENAME}.spv)
  endforeach()

  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${target}_spvs/${resource_name}.hpp
    COMMAND ${CMAKE_COMMAND} -P ${PROJECT_SOURCE_DIR}/CMake/Spv2Header.cmake ${CMAKE_CURRENT_BINARY_DIR}/${target}_spvs/${resource_name}.hpp ${SPV_FILES}
    DEPENDS ${SPV_FILES}
  )

  add_library(${target}_${resource_name} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/${target}_spvs/${resource_name}.hpp)

  target_include_directories(${target}_${resource_name} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/${target}_spvs>)

  target_link_libraries(${target} ${target}_${resource_name})
endfunction()

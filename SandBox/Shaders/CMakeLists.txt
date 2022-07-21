
set(SHADERS
  hello_triangle.vert
  hello_triangle.frag
)

foreach(SHADER in ${SHADERS})
  get_filename_component(FILENAME ${SHADER} NAME)
  message(${FILENAME})
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}.spv
    COMMAND Vulkan::glslc ${SHADER} -o ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}.spv
    DEPENDS ${SHADER}
    COMMENT "Compiling ${FILENAME}"
  )

  list(APPEND SPV_SHADERS ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}.spv)

endforeach()

add_custom_target(shaders ALL DEPENDS ${SPV_SHADERS})

# Creates C resources file from files in given directory
function(create_resources dir output)
  # Create empty output file
  file(WRITE ${output} "#pragma once\n")
  # Collect input files
  file(GLOB bins ${dir}/*.glsl ${dir}/*.spv)
  # Iterate through input files
  foreach(bin ${bins})
    message("bin: ${bin}")
    # Get short filename
    string(REGEX MATCH "([^/]+)$" filename ${bin})
    # Replace filename spaces & extension separator for C compatibility
    string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
    # Read hex data from file
    file(READ ${bin} filedata HEX)
    # Convert hex data for C compatibility
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
    # Append data to output file
    file(APPEND ${output} "alignas(4) const unsigned char ${filename}[] = {${filedata}};\nconst unsigned ${filename}_size = sizeof(${filename});\n")
  endforeach()
endfunction()

create_resources(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/ShaderSource.hpp)

add_library(ShaderSource INTERFACE
  ${CMAKE_CURRENT_BINARY_DIR}/ShaderSource.hpp
)

target_include_directories(ShaderSource INTERFACE
  $<BUILD_INTERFACE:${BUILD_INTERFACE}>
)

add_dependencies(ShaderSource shaders)

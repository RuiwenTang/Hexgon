

# Creates C resources file from files in given directory
function(create_resources dir output)
  # Create empty output file
  file(WRITE ${output} "#pragma once\n")
  # Collect input files
  file(GLOB bins ${dir}/*.glsl ${dir}/*.spv)
  message("create resources")
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

create_resources(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/ShaderSource.hpp)

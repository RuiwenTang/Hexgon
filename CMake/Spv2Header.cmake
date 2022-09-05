# Creates C resources file from files in given directory
function(create_resources output)
  # Create empty output file
  file(WRITE ${output} "#pragma once\n")

  # Collect input files
  message("create resources ${CMAKE_ARGC}")

  math(EXPR SPV_END_INDEX "${CMAKE_ARGC} - 1")

  # Iterate through input files
  foreach(index RANGE 4 ${SPV_END_INDEX} 1)
    message("bin: ${CMAKE_ARGV${index}}")

    # Get short filename
    string(REGEX MATCH "([^/]+)$" filename ${CMAKE_ARGV${index}})

    # Replace filename spaces & extension separator for C compatibility
    string(REGEX REPLACE "\\.| |-" "_" filename ${filename})

    # Read hex data from file
    file(READ ${CMAKE_ARGV${index}} filedata HEX)

    # Convert hex data for C compatibility
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})

    # Append data to output file
    file(APPEND ${output} "alignas(4) const unsigned char ${filename}[] = {${filedata}};\nconst unsigned ${filename}_size = sizeof(${filename});\n")
  endforeach()
endfunction()

create_resources(${CMAKE_ARGV3})
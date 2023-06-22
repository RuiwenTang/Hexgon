cmake_minimum_required(VERSION 3.14.0)

project(Hexgon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if(WIN32)
  set(BUILD_SHARED_LIBS TRUE)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
elseif(UNIX AND NOT APPLE)
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# engine
add_subdirectory(Engine)

# sand-box
add_subdirectory(SandBox)


# source group for Xcode and Visual Studio
get_target_property(HEXGON_SRC Hexgon SOURCES)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEXGON_SRC})

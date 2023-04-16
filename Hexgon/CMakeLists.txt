
add_library(Hexgon
    include/hexgon/core/application.hpp
    src/core/application.cc
)

target_include_directories(
    Hexgon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)


# deps
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(Hexgon PRIVATE glfw)


add_library(Hexgon
    include/hexgon/core/application.hpp
    include/hexgon/core/base.hpp
    include/hexgon/core/log.hpp
    include/hexgon/core/window.hpp
    src/core/application.cc
    src/core/log.cc
    src/core/window.cc
)

target_include_directories(
    Hexgon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)


# deps
# glfw
find_package(glfw3 CONFIG REQUIRED)
# log
find_package(spdlog CONFIG REQUIRED)

target_link_libraries(Hexgon PRIVATE glfw)
target_link_libraries(Hexgon PUBLIC spdlog::spdlog)


add_library(Hexgon)

target_include_directories(
    Hexgon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

target_sources(Hexgon 
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Application.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Hexgon.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Application.cc
)

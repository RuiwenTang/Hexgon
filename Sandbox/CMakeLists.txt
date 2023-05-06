
set(ASSETS_DIR "${CMAKE_CURRENT_LIST_DIR}/assets")

configure_file (config.hpp.in config.hpp)


add_executable(hello_world
        example_layer.cc
        example_layer.hpp
        hello_world.cc
)

target_include_directories(hello_world 
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(hello_world Hexgon)
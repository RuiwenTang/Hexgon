
add_executable(hello_world
        example_layer.cc
        example_layer.hpp
        hello_world.cc
)

target_include_directories(hello_world PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(hello_world Hexgon)
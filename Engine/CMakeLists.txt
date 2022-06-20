
add_library(Hexgon SHARED)

target_include_directories(
    Hexgon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

target_include_directories(
    Hexgon PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
)

if(WIN32)
    target_compile_options(
        Hexgon
        PUBLIC
        PRIVATE
        /EHs-c- # disable exceptions
    )
else()
    target_compile_options(
        Hexgon
        PUBLIC
        PRIVATE
        -fno-exceptions
        -fvisibility=hidden
    )
endif()

target_sources(Hexgon 
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Application.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Hexgon.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Log.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Macro.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Application.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Log.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/LogPrivate.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/LogPrivate.hpp
)

target_link_libraries(Hexgon spdlog::spdlog)

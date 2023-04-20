
add_library(Hexgon
    include/hexgon/core/application.hpp
    include/hexgon/core/base.hpp
    include/hexgon/core/log.hpp
    include/hexgon/core/platform.hpp
    include/hexgon/core/window.hpp
    include/hexgon/event/event.hpp
    include/hexgon/event/key_event.hpp
    include/hexgon/event/window_event.hpp
    include/hexgon/render/render_system.hpp
    include/hexgon/render/swapchain.hpp
    src/core/application.cc
    src/core/log.cc
    src/core/window.cc
    src/render/render_system.cc
)

if(APPLE)
    target_sources(Hexgon PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/render_system_mtl.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/render_system_mtl.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/swapchain_mtl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/swapchain_mtl.mm
    )

    target_link_libraries(Hexgon PRIVATE "-framework Metal")
    target_link_libraries(Hexgon PRIVATE "-framework MetalKit")
endif()

target_include_directories(
    Hexgon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

target_include_directories(
    Hexgon PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# deps
# glfw
find_package(glfw3 CONFIG REQUIRED)

# log
find_package(spdlog CONFIG REQUIRED)

# glm
find_package(glm CONFIG REQUIRED)

target_link_libraries(Hexgon PRIVATE glfw)
target_link_libraries(Hexgon PUBLIC spdlog::spdlog)
target_link_libraries(Hexgon PUBLIC glm::glm)

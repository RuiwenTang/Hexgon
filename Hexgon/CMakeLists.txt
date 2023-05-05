
add_library(Hexgon
    include/hexgon/core/application.hpp
    include/hexgon/core/base.hpp
    include/hexgon/core/layer.hpp
    include/hexgon/core/layer_stack.hpp
    include/hexgon/core/log.hpp
    include/hexgon/core/platform.hpp
    include/hexgon/core/window.hpp
    include/hexgon/event/event.hpp
    include/hexgon/event/key_event.hpp
    include/hexgon/event/window_event.hpp
    include/hexgon/render/command_buffer.hpp
    include/hexgon/render/format.hpp
    include/hexgon/render/render_system.hpp
    include/hexgon/render/render_target.hpp
    include/hexgon/render/renderpass.hpp
    include/hexgon/render/shader.hpp
    include/hexgon/render/swapchain.hpp
    include/hexgon/render/texture.hpp
    include/hexgon/render/types.hpp
    include/hexgon/render/vertex_attribute.hpp
    src/core/application.cc
    src/core/layer_stack.cc
    src/core/log.cc
    src/core/window.cc
    src/render/render_system.cc
    src/render/shader.cc
)

if(APPLE)
    target_sources(Hexgon PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/command_buffer_mtl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/command_buffer_mtl.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/render_system_mtl.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/render_system_mtl.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/renderpass_mtl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/renderpass_mtl.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/shader_mtl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/shader_mtl.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/swapchain_mtl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/swapchain_mtl.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/texture_mtl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/texture_mtl.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/types_mtl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/types_mtl.mm
    )

    target_link_libraries(Hexgon PRIVATE "-framework Metal")
    target_link_libraries(Hexgon PRIVATE "-framework MetalKit")
elseif(WIN32)
    # link vulkan
    find_package(Vulkan REQUIRED)

    target_sources(Hexgon PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/vulkan/render_system_vk.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/vulkan/render_system_vk.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/vulkan/swapchain_vk.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/vulkan/swapchain_vk.hpp
    )

    target_link_libraries(Hexgon PRIVATE Vulkan::Vulkan)
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

# shaderc
find_package(glslang CONFIG REQUIRED)

target_link_libraries(Hexgon PRIVATE glfw)
target_link_libraries(Hexgon PUBLIC spdlog::spdlog)
target_link_libraries(Hexgon PUBLIC glm::glm)
target_link_libraries(Hexgon PRIVATE glslang::glslang)

# source group for Xcode and Visual Studio
get_target_property(HEXGON_SRC Hexgon SOURCES)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEXGON_SRC})

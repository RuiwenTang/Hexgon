
add_library(Hexgon SHARED)

include(${PROJECT_SOURCE_DIR}/CMake/ShaderUtil.cmake)

find_package(Vulkan REQUIRED)

# deps from vckpg
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

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
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
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
            -fvisibility=hidden
        )
    endif()
else()
    target_compile_options(
        Hexgon
        PUBLIC
        PRIVATE
        -fvisibility=hidden
    )
endif()

target_sources(Hexgon
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/Application.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/Event.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/Geometry.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/Layer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/LayerStack.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/Log.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/Material.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/Window.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Hexgon.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Macro.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Object/Camera.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Object/Mesh.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Object/Object3D.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Render/RenderSystem.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Application.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Event.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Geometry.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Geometry/Box.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Geometry/Box.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/LayerStack.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Log.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Material.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Window.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/LogPrivate.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/LogPrivate.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Object/Camera.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Object/Mesh.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Object/Object3D.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Render/RenderSystem.cc
)

# vulkan backend
if (WIN32)
    find_package(Vulkan REQUIRED)

    target_sources(Hexgon PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/Render/Vulkan/RenderSystemVk.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/Render/Vulkan/RenderSystemVk.hpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Render/Vulkan/VulkanUtil.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/Render/Vulkan/VulkanUtil.hpp
    )
    target_link_libraries(Hexgon PRIVATE Vulkan::Vulkan)

endif()

target_link_libraries(Hexgon PRIVATE glfw)
target_link_libraries(Hexgon PUBLIC glm::glm)
target_link_libraries(Hexgon PUBLIC spdlog::spdlog)
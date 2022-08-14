
add_library(Hexgon SHARED)

find_package(Vulkan REQUIRED)

target_include_directories(
    Hexgon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

target_include_directories(
    Hexgon PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${PROJECT_SOURCE_DIR}/third_party/VulkanMemoryAllocator/include
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
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/GraphicsContext.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/Layer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/LayerStack.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/Log.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Core/Window.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/GPU/Buffer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/GPU/Formats.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/GPU/Pipeline.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/GPU/RenderPass.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/GPU/Shader.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/GPU/Texture.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Hexgon.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Hexgon/Macro.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Application.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Event.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/GraphicsContext.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/LayerStack.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Log.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Window.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/Buffer.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/Buffer.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/Buffer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/CommandBuffer.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/CommandBuffer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/Formats.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/Formats.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/FrameData.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/FrameData.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/GraphicsContext.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/GraphicsContext.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/Pipeline.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/Pipeline.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/PipelineBuilder.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/PipelineBuilder.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/RenderPass.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/GPU/VK/RenderPass.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/LogPrivate.cc
    ${CMAKE_CURRENT_LIST_DIR}/src/LogPrivate.hpp
)

target_link_libraries(Hexgon Vulkan::Vulkan)
target_link_libraries(Hexgon glfw)
target_link_libraries(Hexgon glm::glm)
target_link_libraries(Hexgon spdlog::spdlog)
target_link_libraries(Hexgon spirv-reflect-static)
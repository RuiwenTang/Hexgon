/*
 *   Copyright (c) 2022 RuiwenTang
 *   All rights reserved.

 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:

 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.

 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#include "GPU/VK/PipelineBuilder.hpp"

#include "GPU/VK/Formats.hpp"
#include "LogPrivate.hpp"

namespace hexgon::gpu::vk {

static VkShaderModule create_shader_module(VkDevice device, const char* shader, size_t shader_size) {
  VkShaderModuleCreateInfo create_info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
  create_info.codeSize = shader_size;
  create_info.pCode = reinterpret_cast<uint32_t*>(const_cast<char*>(shader));

  VkShaderModule shader_module = VK_NULL_HANDLE;

  if (vkCreateShaderModule(device, &create_info, nullptr, &shader_module) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed create shader modue !!");
  }

  return shader_module;
}

PipelineBuilder::~PipelineBuilder() { CleanUp(); }

void PipelineBuilder::CleanUp() {
  for (auto module : m_shaders) {
    vkDestroyShaderModule(m_device, module, nullptr);
  }
}

VkPipeline PipelineBuilder::Build() {
  InitShaderStage();

  VkPipeline pipeline = VK_NULL_HANDLE;

  return pipeline;
}

void PipelineBuilder::InitShaderStage() {
  for (auto const& shader : m_info.shaders) {
    VkShaderModule shader_module = create_shader_module(m_device, shader.GetSource(), shader.GetSize());

    if (shader_module == VK_NULL_HANDLE) {
      HEX_CORE_ERROR("shader module is null some thing is wrong!");
      continue;
    }

    m_shaders.emplace_back(shader_module);

    VkPipelineShaderStageCreateInfo create_info{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    create_info.stage = Convertor<gpu::Shader::Stage, VkShaderStageFlagBits>::ToVulkan(shader.GetShaderStage());
    create_info.module = shader_module;
    create_info.pName = "main";

    m_shader_stages_info.emplace_back(create_info);
  }
}

}  // namespace hexgon::gpu::vk
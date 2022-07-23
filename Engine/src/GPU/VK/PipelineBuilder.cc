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
  InitInputBindingDesc();
  InitAssemblyState();
  InitMultiSample();
  InitDynamicState();

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

void PipelineBuilder::InitInputBindingDesc() {
  for (auto binding : m_info.vertex_binding) {
    VkVertexInputBindingDescription vk_binding;
    vk_binding.binding = binding.slot;
    vk_binding.stride = binding.stride;

    m_vertex_input_binding.emplace_back(vk_binding);
  }

  for (auto attr : m_info.attr_desc) {
    VkVertexInputAttributeDescription vk_attr;
    vk_attr.binding = attr.slot;
    vk_attr.location = attr.location;
    vk_attr.format = Convertor<gpu::DataType, VkFormat>::ToVulkan(attr.type);
    vk_attr.offset = attr.offset;

    m_vertex_attr_desc.emplace_back(vk_attr);
  }
}

void PipelineBuilder::InitAssemblyState() {
  m_input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  m_input_assembly_info.topology = Convertor<gpu::PrimitiveType, VkPrimitiveTopology>::ToVulkan(m_info.primitive);
}

void PipelineBuilder::InitMultiSample() {
  m_multi_sample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

  m_multi_sample_info.sampleShadingEnable = VK_FALSE;
  m_multi_sample_info.alphaToCoverageEnable = VK_FALSE;
  m_multi_sample_info.alphaToOneEnable = VK_FALSE;
  m_multi_sample_info.minSampleShading = 1.f;

  m_multi_sample_info.rasterizationSamples =
      Convertor<gpu::SampleCount, VkSampleCountFlagBits>::ToVulkan(m_info.sample_count);
}

void PipelineBuilder::InitDynamicState() {
  m_dynamic_state.emplace_back(VK_DYNAMIC_STATE_VIEWPORT);
  m_dynamic_state.emplace_back(VK_DYNAMIC_STATE_SCISSOR);

  m_dynamic_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

  m_dynamic_info.dynamicStateCount = static_cast<uint32_t>(m_dynamic_state.size());
  m_dynamic_info.pDynamicStates = m_dynamic_state.data();
}

}  // namespace hexgon::gpu::vk
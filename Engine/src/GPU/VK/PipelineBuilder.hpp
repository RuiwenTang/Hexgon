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

#ifndef ENGINE_SRC_GPU_VK_PIPELINE_BUILDER_HPP_
#define ENGINE_SRC_GPU_VK_PIPELINE_BUILDER_HPP_

#include <vulkan/vulkan.h>

#include <Hexgon/GPU/Pipeline.hpp>
#include <vector>

namespace hexgon {
namespace gpu {
namespace vk {

class PipelineBuilder final {
 public:
  PipelineBuilder(VkDevice device, gpu::PipelineInfo const& info) : m_device(device), m_info(info) {}
  ~PipelineBuilder();

  VkPipeline Build();

 private:
  void CleanUp();
  void InitShaderStage();
  void InitInputBindingDesc();

 private:
  VkDevice m_device;
  gpu::PipelineInfo m_info;
  std::vector<VkVertexInputBindingDescription> m_vertex_input_binding = {};
  std::vector<VkVertexInputAttributeDescription> m_vertex_attr_desc = {};
  std::vector<VkPipelineColorBlendAttachmentState> m_color_blend_attachment = {};
  std::vector<VkDynamicState> m_dynamic_state = {};
  std::vector<VkPipelineShaderStageCreateInfo> m_shader_stages_info = {};
  VkPipelineVertexInputStateCreateInfo m_vertex_input_info = {};
  VkPipelineInputAssemblyStateCreateInfo m_input_assembly_info = {};
  VkPipelineColorBlendStateCreateInfo m_color_blend_info = {};
  VkPipelineViewportStateCreateInfo m_view_port_info = {};
  VkPipelineDepthStencilStateCreateInfo m_depth_stencil_info = {};
  VkPipelineMultisampleStateCreateInfo m_multi_sample_info = {};
  VkPipelineRasterizationStateCreateInfo m_rasterization_info = {};
  VkPipelineDynamicStateCreateInfo m_dynamic_info = {};
  VkGraphicsPipelineCreateInfo m_create_info = {};

  // temp properties
  std::vector<VkShaderModule> m_shaders;
};

}  // namespace vk
}  // namespace gpu
}  // namespace hexgon

#endif  // ENGINE_SRC_GPU_VK_PIPELINE_BUILDER_HPP_
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

#include <string>

#include "GPU/VK/Formats.hpp"
#include "GPU/VK/RenderPass.hpp"
#include "LogPrivate.hpp"
#include "spirv_reflect.h"

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
  InitPipelineLayout();
  InitInputBindingDesc();
  InitAssemblyState();
  InitMultiSample();
  InitDynamicState();
  InitColorAttachments();
  InitDepthStencilInfo();
  InitRasterizationInfo();

  VkPipeline pipeline = VK_NULL_HANDLE;

  m_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

  // render pass
  m_create_info.renderPass = dynamic_cast<vk::RenderPass*>(m_info.render_pass)->NativeRenderPass();
  m_create_info.subpass = 0;
  // view port
  m_create_info.pViewportState = &m_view_port_info;
  // color blend info
  m_create_info.pColorBlendState = &m_color_blend_info;
  // vertex input
  m_create_info.pVertexInputState = &m_vertex_input_info;
  // input assemble
  m_create_info.pInputAssemblyState = &m_input_assembly_info;
  // rasterization
  m_create_info.pRasterizationState = &m_rasterization_info;
  // multisample
  m_create_info.pMultisampleState = &m_multi_sample_info;
  // depth stencil
  m_create_info.pDepthStencilState = &m_depth_stencil_info;
  // dynamic state
  m_create_info.pDynamicState = &m_dynamic_info;

  if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &m_create_info, nullptr, &pipeline) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed create vulkan backend pipeline");
    exit(-1);
  }

  return pipeline;
}

VkPipelineLayout PipelineBuilder::GetLayout() const { return m_layout; }

std::vector<VkDescriptorSetLayout> const& PipelineBuilder::GetDescriptorSetLayout() const {
  return m_descriptor_set_layout;
}

std::vector<DescriptorSetLayoutData> const& PipelineBuilder::GetDescriptorSetInfo() const { return m_set_info; }

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

  m_create_info.stageCount = static_cast<uint32_t>(m_shader_stages_info.size());
  m_create_info.pStages = m_shader_stages_info.data();
}

void PipelineBuilder::InitPipelineLayout() {
  VkPipelineLayoutCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

  for (auto const& shader : m_info.shaders) {
    SpvReflectShaderModule module = {};
    SpvReflectResult result = spvReflectCreateShaderModule(shader.GetSize(), shader.GetSource(), &module);

    if (result != SPV_REFLECT_RESULT_SUCCESS) {
      HEX_CORE_ERROR("Failed reflect shader !!");
      continue;
    }

    uint32_t count = 0;
    result = spvReflectEnumerateDescriptorSets(&module, &count, nullptr);

    if (result != SPV_REFLECT_RESULT_SUCCESS) {
      HEX_CORE_ERROR("Failed refelct shader descriptors");
      continue;
    } else {
      HEX_CORE_INFO("reflect {} stage with {} sets",
                    shader.GetShaderStage() == Shader::Stage::Vertex ? "vertex" : "fragment", count);
    }

    std::vector<SpvReflectDescriptorSet*> descs(count);

    spvReflectEnumerateDescriptorSets(&module, &count, descs.data());

    for (size_t i_set = 0; i_set < descs.size(); i_set++) {
      auto it = std::find_if(m_set_info.begin(), m_set_info.end(),
                             [i_set](DescriptorSetLayoutData const& d) { return d.set_number == i_set; });

      DescriptorSetLayoutData* set_data = nullptr;
      if (it != m_set_info.end()) {
        set_data = &m_set_info[std::distance(it, m_set_info.begin())];
      } else {
        m_set_info.emplace_back(DescriptorSetLayoutData{});
        set_data = &m_set_info.back();
      }

      SpvReflectDescriptorSet* refl_set = descs[i_set];
      for (uint32_t i_index = 0; i_index < refl_set->binding_count; i_index++) {
        SpvReflectDescriptorBinding* refl_binding = refl_set->bindings[i_index];

        uint32_t i_binding = refl_binding->binding;

        auto it_binding =
            std::find_if(set_data->bindings.begin(), set_data->bindings.end(),
                         [i_binding](VkDescriptorSetLayoutBinding const& vk_b) { return vk_b.binding == i_binding; });

        if (it_binding != set_data->bindings.end()) {
          it_binding->stageFlags |=
              Convertor<gpu::Shader::Stage, VkShaderStageFlagBits>::ToVulkan(shader.GetShaderStage());

          continue;
        }

        VkDescriptorSetLayoutBinding vk_binding{};
        vk_binding.binding = refl_binding->binding;
        vk_binding.descriptorType = static_cast<VkDescriptorType>(refl_binding->descriptor_type);
        vk_binding.descriptorCount = 1;
        for (uint32_t i_dim = 0; i_dim < refl_binding->array.dims_count; i_dim++) {
          vk_binding.descriptorCount *= refl_binding->array.dims[i_dim];
        }

        vk_binding.stageFlags = static_cast<VkShaderStageFlagBits>(module.shader_stage);

        set_data->bindings.emplace_back(vk_binding);
        set_data->binding_names.emplace_back(refl_binding->name);

        HEX_CORE_INFO("Reflect set [{}] binding [{}] with name [{}]", i_set, refl_binding->binding, refl_binding->name);
      }

      set_data->create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      set_data->create_info.bindingCount = refl_set->binding_count;
      set_data->create_info.pBindings = set_data->bindings.data();

      set_data->set_number = refl_set->set;
      set_data->stage = Convertor<gpu::Shader::Stage, VkShaderStageFlagBits>::ToVulkan(shader.GetShaderStage());
    }
  }

  // TODO: merge bindings

  m_descriptor_set_layout.resize(m_set_info.size());

  for (size_t i = 0; i < m_descriptor_set_layout.size(); i++) {
    if (vkCreateDescriptorSetLayout(m_device, &m_set_info[i].create_info, nullptr, &m_descriptor_set_layout[i]) !=
        VK_SUCCESS) {
      HEX_CORE_ERROR("Failed to create descriptor set layout at {}", i);
      exit(-1);
    }
  }

  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  create_info.setLayoutCount = static_cast<uint32_t>(m_descriptor_set_layout.size());
  create_info.pSetLayouts = m_descriptor_set_layout.data();

  if (vkCreatePipelineLayout(m_device, &create_info, nullptr, &m_layout) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed to create pipeline layout!");
    exit(-1);
  }

  m_create_info.layout = m_layout;
}

void PipelineBuilder::InitInputBindingDesc() {
  for (auto binding : m_info.vertex_binding) {
    VkVertexInputBindingDescription vk_binding;
    vk_binding.binding = binding.slot;
    vk_binding.stride = binding.stride;
    vk_binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

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

  m_vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

  m_vertex_input_info.vertexBindingDescriptionCount = static_cast<uint32_t>(m_vertex_input_binding.size());
  m_vertex_input_info.pVertexBindingDescriptions = m_vertex_input_binding.data();

  m_vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_vertex_attr_desc.size());
  m_vertex_input_info.pVertexAttributeDescriptions = m_vertex_attr_desc.data();
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

void PipelineBuilder::InitColorAttachments() {
  for (auto const& desc : m_info.color_attachment) {
    VkPipelineColorBlendAttachmentState vk_desc;

    vk_desc.blendEnable = desc.blending;

    vk_desc.colorBlendOp = Convertor<gpu::BlendOperation, VkBlendOp>::ToVulkan(desc.color.op);
    vk_desc.srcColorBlendFactor = Convertor<gpu::BlendFactor, VkBlendFactor>::ToVulkan(desc.color.src);
    vk_desc.dstColorBlendFactor = Convertor<gpu::BlendFactor, VkBlendFactor>::ToVulkan(desc.color.dst);

    vk_desc.alphaBlendOp = Convertor<gpu::BlendOperation, VkBlendOp>::ToVulkan(desc.alpha.op);
    vk_desc.srcAlphaBlendFactor = Convertor<gpu::BlendFactor, VkBlendFactor>::ToVulkan(desc.alpha.src);
    vk_desc.dstAlphaBlendFactor = Convertor<gpu::BlendFactor, VkBlendFactor>::ToVulkan(desc.alpha.dst);

    vk_desc.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    m_color_blend_attachment.emplace_back(vk_desc);
  }

  m_color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

  m_color_blend_info.attachmentCount = static_cast<uint32_t>(m_color_blend_attachment.size());
  m_color_blend_info.pAttachments = m_color_blend_attachment.data();

  m_view_port_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  m_view_port_info.viewportCount = m_color_blend_info.attachmentCount;
  m_view_port_info.scissorCount = m_color_blend_info.attachmentCount;
  m_view_port_info.flags = 0;
}

void PipelineBuilder::InitDepthStencilInfo() {
  m_depth_stencil_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

  m_depth_stencil_info.minDepthBounds = 0.f;
  m_depth_stencil_info.maxDepthBounds = 1.f;

  // TODO: support stencil test
  m_depth_stencil_info.stencilTestEnable = VK_FALSE;
  m_depth_stencil_info.depthBoundsTestEnable = VK_FALSE;

  if (m_info.depth_attachment.empty()) {
    m_depth_stencil_info.depthTestEnable = VK_FALSE;
    m_depth_stencil_info.depthWriteEnable = VK_FALSE;
    m_depth_stencil_info.depthCompareOp = VK_COMPARE_OP_ALWAYS;
  } else {
    m_depth_stencil_info.depthTestEnable = VK_TRUE;
    m_depth_stencil_info.depthWriteEnable = m_info.depth_attachment[0].depth_writable;
    m_depth_stencil_info.depthCompareOp =
        Convertor<gpu::CompareFunction, VkCompareOp>::ToVulkan(m_info.depth_attachment[0].compare);
  }
}

void PipelineBuilder::InitRasterizationInfo() {
  m_rasterization_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  m_rasterization_info.depthClampEnable = VK_FALSE;
  m_rasterization_info.rasterizerDiscardEnable = VK_FALSE;
  m_rasterization_info.lineWidth = 1.f;

  m_rasterization_info.polygonMode = VK_POLYGON_MODE_FILL;
  m_rasterization_info.cullMode = VK_CULL_MODE_NONE;
  m_rasterization_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

  m_rasterization_info.depthBiasEnable = VK_FALSE;
}

}  // namespace hexgon::gpu::vk
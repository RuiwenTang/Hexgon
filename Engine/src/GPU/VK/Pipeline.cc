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

#include "GPU/VK/Pipeline.hpp"

#include "GPU/VK/Buffer.hpp"
#include "GPU/VK/GraphicsContext.hpp"
#include "GPU/VK/Texture.hpp"
#include "LogPrivate.hpp"

namespace hexgon::gpu::vk {

static VkWriteDescriptorSet write_descriptor_set(VkDescriptorSet dst_set, VkDescriptorType type, uint32_t binding,
                                                 VkDescriptorBufferInfo* buffer_info, uint32_t descriptor_count) {
  VkWriteDescriptorSet write_set{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};

  write_set.dstSet = dst_set;
  write_set.descriptorType = type;
  write_set.dstBinding = binding;
  write_set.pBufferInfo = buffer_info;
  write_set.descriptorCount = descriptor_count;

  return write_set;
}

static VkWriteDescriptorSet write_descriptor_set(VkDescriptorSet dst_set, VkDescriptorType type, uint32_t binding,
                                                 VkDescriptorImageInfo* image_info, uint32_t descriptor_count) {
  VkWriteDescriptorSet write_set{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};

  write_set.dstSet = dst_set;
  write_set.descriptorType = type;
  write_set.dstBinding = binding;
  write_set.pImageInfo = image_info;
  write_set.descriptorCount = descriptor_count;

  return write_set;
}

Pipeline::Pipeline(VkDevice device, VkPipeline pipeline, VkPipelineLayout layout,
                   std::vector<VkDescriptorSetLayout> set_layout, std::vector<DescriptorSetLayoutData> set_info)
    : m_device(device),
      m_pipeline(pipeline),
      m_layout(layout),
      m_set_layout(std::move(set_layout)),
      m_set_info(std::move(set_info)) {}

Pipeline::~Pipeline() { CleanUp(); }

void Pipeline::UpdateDescriptorSet(uint32_t slot, std::vector<DescriptorBinding> const& bindings) {
  if (slot >= m_set_info.size()) {
    HEX_CORE_ERROR("Slot: {} is not present in this pipeline", slot);
    return;
  }

  auto vk_set = m_vk_context->ObtainUniformBufferSet(m_set_layout[slot]);

  if (vk_set == VK_NULL_HANDLE) {
    HEX_CORE_ERROR("Failed obtain descriptor set for slot {}", slot);
    return;
  }

  // update descriptor set
  std::vector<VkWriteDescriptorSet> write_sets;
  std::vector<VkDescriptorBufferInfo> buffer_infos;
  std::vector<VkDescriptorImageInfo> image_infos;

  for (uint32_t i = 0; i < static_cast<uint32_t>(bindings.size()); i++) {
    // TODO support ImageSet
    if (bindings[i].type == gpu::DescriptorBinding::Type::kUniformBuffer) {
      auto vk_ubo = dynamic_cast<vk::UniformBuffer*>(bindings[i].data.ubo);
      if (vk_ubo == nullptr) {
        HEX_CORE_ERROR("uniform buffer object is error");
        return;
      }

      VkDescriptorBufferInfo buff_info{};
      buff_info.buffer = vk_ubo->NativeBuffer();
      buff_info.offset = vk_ubo->NativeOffset();
      buff_info.range = vk_ubo->GetBufferSize();

      buffer_infos.emplace_back(buff_info);

      write_sets.emplace_back(
          write_descriptor_set(vk_set, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, i, &buffer_infos.back(), 1));
    } else if (bindings[i].type == gpu::DescriptorBinding::Type::kSampledTexture) {
      auto vk_img = dynamic_cast<vk::Texture*>(bindings[i].data.texture);
      if (vk_img == nullptr) {
        HEX_CORE_ERROR("texture buffer object is error");
        return;
      }

      vk_img->PrepareForDraw();

      VkDescriptorImageInfo image_info{};
      image_info.sampler = vk_img->Get2dSampler();
      image_info.imageView = vk_img->GetImageView();
      image_info.imageLayout = vk_img->GetImageLayout();

      image_infos.emplace_back(image_info);

      write_sets.emplace_back(
          write_descriptor_set(vk_set, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, i, &image_infos.back(), 1));
    }
  }

  vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(write_sets.size()), write_sets.data(), 0, VK_NULL_HANDLE);

  // bind descriptor set
  auto vk_cmd = dynamic_cast<vk::CommandBuffer*>(m_vk_context->CurrentCommandBuffer())->GetCMD();
  vkCmdBindDescriptorSets(vk_cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_layout, slot, 1, &vk_set, 0, nullptr);
}

void Pipeline::CleanUp() {
  vkDestroyPipelineLayout(m_device, m_layout, nullptr);

  for (auto desc : m_set_layout) {
    vkDestroyDescriptorSetLayout(m_device, desc, nullptr);
  }

  vkDestroyPipeline(m_device, m_pipeline, nullptr);
}

}  // namespace hexgon::gpu::vk
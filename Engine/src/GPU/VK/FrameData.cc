#include "FrameData.hpp"
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

#include <array>

#include "GPU/VK/FrameData.hpp"
#include "LogPrivate.hpp"

namespace hexgon::gpu::vk {

void Frame::Init() {
  AppendNewPoll();
  m_pool_index = 0;
}

void Frame::CleanUp() {
  for (auto const& pool : m_pool_list) {
    vkResetDescriptorPool(m_device, pool, 0);
    vkDestroyDescriptorPool(m_device, pool, nullptr);
  }

  m_pool_list.clear();
  m_pool_index = -1;
}

void Frame::BeginFrame() {
  for (auto const& pool : m_pool_list) {
    vkResetDescriptorPool(m_device, pool, 0);
  }

  m_pool_index = static_cast<int32_t>(m_pool_list.size()) - 1;
}

VkDescriptorSet Frame::ObtainUniformBufferSet(VkDescriptorSetLayout layout) {
  VkDescriptorSetAllocateInfo allocate_info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};

  allocate_info.descriptorPool = m_pool_list[m_pool_index];
  allocate_info.pSetLayouts = &layout;
  allocate_info.descriptorSetCount = 1;

  VkDescriptorSet vk_set = VK_NULL_HANDLE;

  auto result = vkAllocateDescriptorSets(m_device, &allocate_info, &vk_set);

  if (result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_INITIALIZATION_FAILED) {
    AppendNewPoll();
    m_pool_index++;

    allocate_info.descriptorPool = m_pool_list[m_pool_index];

    result = vkAllocateDescriptorSets(m_device, &allocate_info, &vk_set);
  }

  if (result != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed to allocate descriptor set for uniform buffer object");
  }

  return vk_set;
}

void Frame::AppendNewPoll() {
  std::array<VkDescriptorPoolSize, 3> pool_size{};
  pool_size[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  pool_size[0].descriptorCount = FRAME_DEFAULT_POOL_SIZE;
  pool_size[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  pool_size[1].descriptorCount = FRAME_DEFAULT_POOL_SIZE;
  pool_size[2].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
  pool_size[2].descriptorCount = FRAME_DEFAULT_POOL_SIZE;

  VkDescriptorPoolCreateInfo create_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  create_info.poolSizeCount = static_cast<uint32_t>(pool_size.size());
  create_info.pPoolSizes = pool_size.data();
  create_info.maxSets = FRAME_DEFAULT_POOL_SIZE;

  VkDescriptorPool new_pool = VK_NULL_HANDLE;

  if (vkCreateDescriptorPool(m_device, &create_info, nullptr, &new_pool) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed create new DescriptorPool!");
    exit(-1);
  }

  m_pool_list.emplace_back(new_pool);
}

void FrameData::Init(VkDevice device, uint32_t total_frame) {
  m_total_frame = total_frame;
  m_current_frame = 0;

  for (uint32_t i = 0; i < m_total_frame; i++) {
    auto frame = std::make_unique<Frame>(device);
    frame->Init();

    m_frames.emplace_back(std::move(frame));
  }

  HEX_CORE_INFO("FrameData allocate [{}] frames", total_frame);
}

void FrameData::CleanUp() {
  for (auto const& frame : m_frames) {
    frame->CleanUp();
  }
}

void FrameData::NextFrame(uint32_t frame_index) { m_current_frame = frame_index; }

void FrameData::ResetCurrentFrame() { m_frames[m_current_frame]->BeginFrame(); }

VkDescriptorSet FrameData::ObtainUniformBufferSet(VkDescriptorSetLayout layout) {
  return m_frames[m_current_frame]->ObtainUniformBufferSet(layout);
}

}  // namespace hexgon::gpu::vk
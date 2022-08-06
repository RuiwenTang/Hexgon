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

namespace hexgon::gpu::vk {

Pipeline::Pipeline(VkDevice device, VkPipeline pipeline, VkPipelineLayout layout,
                   std::vector<VkDescriptorSetLayout> set_layout)
    : m_device(device), m_pipeline(pipeline), m_layout(layout), m_set_layout(std::move(set_layout)) {}

Pipeline::~Pipeline() { CleanUp(); }

void Pipeline::UpdateDescriptorSet(const std::string &name, void *data, size_t data_size) {}

void Pipeline::CleanUp() {
  vkDestroyPipelineLayout(m_device, m_layout, nullptr);

  for (auto desc : m_set_layout) {
    vkDestroyDescriptorSetLayout(m_device, desc, nullptr);
  }

  vkDestroyPipeline(m_device, m_pipeline, nullptr);
}

}  // namespace hexgon::gpu::vk
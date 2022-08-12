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

#ifndef ENGINE_SRC_GPU_VK_PIPELINE_HPP_
#define ENGINE_SRC_GPU_VK_PIPELINE_HPP_

#include <vulkan/vulkan.h>

#include <Hexgon/GPU/Pipeline.hpp>
#include <vector>

namespace hexgon {
namespace gpu {
namespace vk {

struct DescriptorSetLayoutData {
  uint32_t set_number;
  VkDescriptorSetLayoutCreateInfo create_info;
  std::vector<VkDescriptorSetLayoutBinding> bindings;
  std::vector<std::string> binding_names;
  VkShaderStageFlags stage;
};

class GraphicsContext;

class Pipeline : public gpu::Pipeline {
 public:
  Pipeline(VkDevice device, VkPipeline pipeline, VkPipelineLayout layout, std::vector<VkDescriptorSetLayout> set_layout,
           std::vector<DescriptorSetLayoutData> set_info);
  ~Pipeline() override;

  void UpdateDescriptorSet(uint32_t slot, std::vector<DescriptorBinding> const& bindings) override;

  void SetContext(GraphicsContext* ctx) { m_vk_context = ctx; }

  VkPipeline NativePipeline() const { return m_pipeline; }

 private:
  void CleanUp();

 private:
  VkDevice m_device;
  VkPipeline m_pipeline;
  VkPipelineLayout m_layout;
  std::vector<VkDescriptorSetLayout> m_set_layout;
  std::vector<DescriptorSetLayoutData> m_set_info;
  GraphicsContext* m_vk_context = nullptr;
};

}  // namespace vk
}  // namespace gpu
}  // namespace hexgon

#endif  // ENGINE_SRC_GPU_VK_PIPELINE_HPP_
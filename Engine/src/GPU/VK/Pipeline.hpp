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

class Pipeline : public gpu::Pipeline {
 public:
  Pipeline(VkDevice device, VkPipeline pipeline, VkPipelineLayout layout,
           std::vector<VkDescriptorSetLayout> set_layout);
  ~Pipeline() override;

  void SetInt(std::string const& name, int32_t value) override;

  void SetFloat(std::string const& name, float value) override;
  void SetFloat2(std::string const& name, glm::vec2 const& value) override;
  void SetFloat3(std::string const& name, glm::vec3 const& value) override;
  void SetFloat4(std::string const& name, glm::vec4 const& value) override;

  void SetMat4(std::string const& name, glm::mat4 const& value) override;

  VkPipeline NativePipeline() const { return m_pipeline; }

 private:
  void CleanUp();

 private:
  VkDevice m_device;
  VkPipeline m_pipeline;
  VkPipelineLayout m_layout;
  std::vector<VkDescriptorSetLayout> m_set_layout;
};

}  // namespace vk
}  // namespace gpu
}  // namespace hexgon

#endif  // ENGINE_SRC_GPU_VK_PIPELINE_HPP_
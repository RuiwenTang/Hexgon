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

#ifndef ENGINE_SRC_GPU_VK_FRAME_DATA_HPP_
#define ENGINE_SRC_GPU_VK_FRAME_DATA_HPP_

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <memory>
#include <vector>

namespace hexgon {
namespace gpu {
namespace vk {

class Frame final {
  enum {
    FRAME_DEFAULT_POOL_SIZE = 1024,
  };

 public:
  Frame(VkDevice device) : m_device(device), m_pool_list(), m_pool_index(-1) {}
  ~Frame() = default;

  void Init();
  void CleanUp();

  void BeginFrame();

  VkDescriptorSet ObtainUniformBufferSet(VkDescriptorSetLayout layout);

 private:
  void AppendNewPoll();

 private:
  VkDevice m_device;
  std::vector<VkDescriptorPool> m_pool_list;
  int32_t m_pool_index;
};

class FrameData final {
 public:
  FrameData() = default;

  ~FrameData() = default;

  void Init(VkDevice device, uint32_t total_frame);

  void CleanUp();

  void NextFrame(uint32_t frame_index);
  void ResetCurrentFrame();

  VkDescriptorSet ObtainUniformBufferSet(VkDescriptorSetLayout layout);

 private:
  uint32_t m_total_frame = 0;
  uint32_t m_current_frame = 0;
  std::vector<std::unique_ptr<Frame>> m_frames;
};

}  // namespace vk
}  // namespace gpu
}  // namespace hexgon

#endif  // ENGINE_SRC_GPU_VK_FRAME_DATA_HPP_
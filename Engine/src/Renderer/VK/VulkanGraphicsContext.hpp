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

#ifndef ENGINE_SRC_RENDERER_VULKAN_GRAPHICS_CONTEXT_HPP_
#define ENGINE_SRC_RENDERER_VULKAN_GRAPHICS_CONTEXT_HPP_

#include <vulkan/vulkan.h>

#include <Hexgon/GraphicsContext.hpp>
#include <vector>

namespace hexgon {

class VulkanGraphicsContext : public GraphicsContext {
  struct ImageWrapper {
    VkImage image = {};
    VkImageView image_view = {};
    VkDeviceMemory memory = {};
    VkFormat format = {};
  };

 public:
  VulkanGraphicsContext(void* window);
  ~VulkanGraphicsContext() override;

  void Init() override;
  void BeginFrame(glm::vec4 const& clear_color) override;
  void SwapBuffers() override;

 private:
  void InitVkInstance();
  void InitVkSurface();
  void PickPhysicalDevice();
  void CreateVkDevice();
  void CreateVkSwapchain();
  void CreateSwapchainImageViews();
  void CreateCommandPool();
  void CreateCommandBuffer();
  void CreateSyncObjects();
  void CreateRenderPass();
  void CreateFramebuffer();

  uint32_t GetMemroyType(uint32_t type_bits, VkMemoryPropertyFlags properties);

 private:
  void* m_window;
  VkInstance m_vk_instance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT m_debug_messenger = VK_NULL_HANDLE;
  VkSurfaceKHR m_vk_surface = VK_NULL_HANDLE;
  VkPhysicalDevice m_phy_device = VK_NULL_HANDLE;
  VkPhysicalDeviceProperties m_phy_props = {};
  int32_t m_graphic_queue_index = -1;
  int32_t m_present_queue_index = -1;
  VkDevice m_device = VK_NULL_HANDLE;
  VkQueue m_graphic_queue = VK_NULL_HANDLE;
  VkQueue m_present_queue = VK_NULL_HANDLE;
  VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
  VkFormat m_swapchain_format = {};
  VkSampleCountFlagBits m_sample_count = VK_SAMPLE_COUNT_1_BIT;
  VkExtent2D m_swapchain_extent = {};
  std::vector<VkImageView> m_swapchain_views = {};
  std::vector<ImageWrapper> m_sampler_images = {};
  std::vector<ImageWrapper> m_depth_images = {};
  VkCommandPool m_cmd_pool = {};
  std::vector<VkCommandBuffer> m_cmds = {};
  std::vector<VkFence> m_cmd_fences = {};
  std::vector<VkSemaphore> m_present_semaphore = {};
  std::vector<VkSemaphore> m_render_semaphore = {};
  VkRenderPass m_render_pass = {};
  std::vector<VkFramebuffer> m_framebuffers = {};
  uint32_t m_current_frame = 0;
  uint32_t m_frame_index = 0;
};

}  // namespace hexgon

#endif  // ENGINE_SRC_RENDERER_VULKAN_GRAPHICS_CONTEXT_HPP_
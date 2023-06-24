/*
 *   Copyright (c) 2023 RuiwenTang
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

#pragma once

#include <vulkan/vulkan.h>

#include <tuple>
#include <vector>

namespace hexgon {

struct PhysicalDeviceInfo {
  VkPhysicalDevice device = {};
  uint32_t graphic_queue_index = {};
  uint32_t present_queue_index = {};
};

class VulkanUtil {
 public:
  VulkanUtil() = delete;
  ~VulkanUtil() = delete;

  static std::tuple<VkInstance, VkDebugReportCallbackEXT> CreateInstance(bool debug);

  static PhysicalDeviceInfo QueryDevice(VkInstance vk_instance, VkSurfaceKHR vk_surface);

  static VkFormat PickSurfaceFormat(VkPhysicalDevice device, VkSurfaceKHR surface);

  static VKAPI_ATTR VkBool32 VKAPI_CALL ValidationCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT type,
                                                           uint64_t object, size_t location, int32_t message_code,
                                                           const char* layer_prefix, const char* message,
                                                           void* user_data);
};

}  // namespace hexgon

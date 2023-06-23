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

#include "Render/Vulkan/VulkanUtil.hpp"

#include <GLFW/glfw3.h>

#include "LogPrivate.hpp"

namespace hexgon {

VkInstance VulkanUtil::CreateInstance() {
  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pEngineName = "Hexgon Engine";
  app_info.pApplicationName = "Hexgon";
  app_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
  app_info.apiVersion = VK_API_VERSION_1_3;

  VkInstanceCreateInfo desc{};
  desc.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  desc.pApplicationInfo = &app_info;

  // TODO support vulkan validation layer
  uint32_t glfw_extension_count = 0;
  const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  std::vector<const char*> required_extensions{};

  for (uint32_t i = 0; i < glfw_extension_count; i++) {
    HEX_CORE_INFO("vk load extension: {}", glfw_extensions[i]);
    required_extensions.emplace_back(glfw_extensions[i]);
  }

  desc.enabledExtensionCount = required_extensions.size();
  desc.ppEnabledExtensionNames = required_extensions.data();

  VkInstance result = {};

  VkResult ret = vkCreateInstance(&desc, nullptr, &result);

  if (ret != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed create Vulkan instance");
    return nullptr;
  }

  return result;
}

PhysicalDeviceInfo VulkanUtil::QueryDevice(VkInstance vk_instance, VkSurfaceKHR vk_surface) {
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(vk_instance, &device_count, nullptr);

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(vk_instance, &device_count, devices.data());

  PhysicalDeviceInfo result{};

  if (devices.empty()) {
    HEX_CORE_ERROR("No available vulkan gpu in system.");
    return result;
  }
  int32_t graphic_queue_family = -1;
  int32_t present_queue_family = -1;

  for (size_t i = 0; i < devices.size(); i++) {
    uint32_t queue_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queue_count, nullptr);

    std::vector<VkQueueFamilyProperties> properties(queue_count);

    vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queue_count, properties.data());

    for (size_t j = 0; j < properties.size(); j++) {
      if (properties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        graphic_queue_family = j;
      }

      VkBool32 present_support = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(devices[i], j, vk_surface, &present_support);

      if (present_support) {
        present_queue_family = j;
      }

      if (graphic_queue_family >= 0 && present_queue_family >= 0) {
        break;
      }
    }

    if (graphic_queue_family >= 0 && present_queue_family >= 0) {
      result.device = devices[i];
      result.graphic_queue_index = graphic_queue_family;
      result.present_queue_index = present_queue_family;
      break;
    } else {
      graphic_queue_family = -1;
      present_queue_family = -1;
    }
  }

  return result;
}

VkFormat VulkanUtil::PickSurfaceFormat(VkPhysicalDevice device, VkSurfaceKHR surface) {
  uint32_t surface_format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &surface_format_count, nullptr);

  std::vector<VkSurfaceFormatKHR> all_formats(surface_format_count);
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &surface_format_count, all_formats.data());

  if (all_formats.size() == 1 && all_formats[0].format == VK_FORMAT_UNDEFINED) {
    return VK_FORMAT_R8G8B8A8_UNORM;
  }

  for (auto format : all_formats) {
    if ((format.format == VK_FORMAT_R8G8B8A8_UNORM || format.format == VK_FORMAT_B8G8R8A8_UNORM) &&
        format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
      return format.format;
    }
  }

  return all_formats[0].format;
}

}  // namespace hexgon

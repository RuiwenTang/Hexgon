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

#include "Render/Vulkan/RenderSystemVk.hpp"

#include <GLFW/glfw3.h>

#include <Hexgon/Core/Window.hpp>
#include <set>
#include <vector>

#include "LogPrivate.hpp"
#include "Render/Vulkan/VulkanUtil.hpp"

namespace hexgon {

std::unique_ptr<RenderSystem> RenderSystemVk::Init(Window* window) {
  std::unique_ptr<RenderSystemVk> ret;
  // step 1 create vulkan instance
  auto instance = VulkanUtil::CreateInstance();

  if (!instance) {
    return ret;
  }

  // step 2 create vulkan surface
  VkSurfaceKHR surface{};
  if (glfwCreateWindowSurface(instance, reinterpret_cast<GLFWwindow*>(window->GetNativeWindow()), nullptr, &surface) !=
      VK_SUCCESS) {
    HEX_CORE_ERROR("Failed create vulkan display surface");
    return ret;
  }

  // step 3 query gpu device
  auto device_info = VulkanUtil::QueryDevice(instance, surface);

  if (!device_info.device) {
    HEX_CORE_ERROR("Can not find usable vulkan device.");
    return ret;
  }

  ret = std::make_unique<RenderSystemVk>();

  if (!ret->InitVulkan(instance, surface, device_info)) {
    ret->ShutDown();
    ret.reset();

    return ret;
  }

  return ret;
}

void RenderSystemVk::ShutDown() {
  if (m_device) {
    vkDestroyDevice(m_device, nullptr);
    m_device = nullptr;
  }

  if (m_vk_surface) {
    vkDestroySurfaceKHR(m_vk_instance, m_vk_surface, nullptr);
    m_vk_surface = nullptr;
  }

  if (m_vk_instance) {
    vkDestroyInstance(m_vk_instance, nullptr);
    m_vk_instance = nullptr;
  }
}

bool RenderSystemVk::InitVulkan(VkInstance instance, VkSurfaceKHR surface, const PhysicalDeviceInfo& device_info) {
  m_vk_instance = instance;
  m_vk_surface = surface;
  m_phy_device = device_info.device;

  // create logical device and queue
  std::vector<VkDeviceQueueCreateInfo> queue_create_info{};

  std::set<uint32_t> queue_families = {
      device_info.graphic_queue_index,
      device_info.present_queue_index,
  };

  float queue_priority = 1.f;

  for (auto family : queue_families) {
    VkDeviceQueueCreateInfo create_info{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    create_info.queueFamilyIndex = family;
    create_info.queueCount = 1;
    create_info.pQueuePriorities = &queue_priority;

    queue_create_info.emplace_back(create_info);
  }

  VkPhysicalDeviceFeatures device_features{};

  device_features.samplerAnisotropy = VK_TRUE;
  device_features.sampleRateShading = VK_TRUE;

  std::vector<const char*> device_extension{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  {
    uint32_t count;
    vkEnumerateDeviceExtensionProperties(m_phy_device, nullptr, &count, nullptr);

    std::vector<VkExtensionProperties> properties(count);
    vkEnumerateDeviceExtensionProperties(m_phy_device, nullptr, &count, properties.data());

    auto it = std::find_if(properties.begin(), properties.end(), [](VkExtensionProperties prop) {
      return std::strcmp(prop.extensionName, "VK_KHR_portability_subset") == 0;
    });

    if (it != properties.end()) {
      // VUID-VkDeviceCreateInfo-pProperties-04451
      device_extension.emplace_back("VK_KHR_portability_subset");
    }
  }

  VkDeviceCreateInfo create_info{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
  create_info.pQueueCreateInfos = queue_create_info.data();
  create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_info.size());
  create_info.pEnabledFeatures = &device_features;
  create_info.enabledExtensionCount = static_cast<uint32_t>(device_extension.size());
  create_info.ppEnabledExtensionNames = device_extension.data();

  if (vkCreateDevice(m_phy_device, &create_info, nullptr, &m_device) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed create logical vulkan device.");
    return false;
  }

  vkGetDeviceQueue(m_device, device_info.graphic_queue_index, 0, &m_graphic_queue);
  vkGetDeviceQueue(m_device, device_info.present_queue_index, 0, &m_present_queue);

  return true;
}

}  // namespace hexgon

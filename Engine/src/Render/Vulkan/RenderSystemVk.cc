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
#include "Render/Vulkan/SwapChainVk.hpp"
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

std::unique_ptr<SwapChain> RenderSystemVk::CreateSwapChain() {
  std::unique_ptr<SwapChain> result{};
  // surface capabilities
  VkSurfaceCapabilitiesKHR surface_capabilities{};
  if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_phy_device, m_vk_surface, &surface_capabilities) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed Get surface capabilities.");
    return result;
  }

  auto format = VulkanUtil::PickSurfaceFormat(m_phy_device, m_vk_surface);

  VkCompositeAlphaFlagBitsKHR surface_composite = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  if (surface_capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) {
    surface_composite = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  } else if (surface_capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR) {
    surface_composite = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
  } else if (surface_capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) {
    surface_composite = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
  } else {
    surface_composite = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
  }

  // create vulkan swapchain
  std::vector<uint32_t> queue_families{};

  queue_families.emplace_back(m_graphic_queue_index);

  if (m_present_queue_index != m_graphic_queue_index) {
    queue_families.emplace_back(m_present_queue_index);
  }

  VkSwapchainKHR vk_swap_chain{};

  VkSwapchainCreateInfoKHR create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = m_vk_surface;
  create_info.minImageCount = surface_capabilities.maxImageCount;
  create_info.imageFormat = format;
  create_info.imageExtent = surface_capabilities.currentExtent;
  create_info.imageArrayLayers = 1;
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  create_info.queueFamilyIndexCount = queue_families.size();
  create_info.pQueueFamilyIndices = queue_families.data();

  if (queue_families.size() == 1) {
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  } else {
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
  }

  create_info.preTransform = surface_capabilities.currentTransform;
  create_info.compositeAlpha = surface_composite;
  create_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;

  if (vkCreateSwapchainKHR(m_device, &create_info, nullptr, &vk_swap_chain) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed to create vulkan swap chain");
    return result;
  }

  result.reset(new SwapChainVk(m_device, vk_swap_chain, surface_capabilities, format));

  return result;
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
  m_graphic_queue_index = device_info.graphic_queue_index;
  m_present_queue_index = device_info.present_queue_index;

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

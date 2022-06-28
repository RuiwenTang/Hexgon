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

#include "Renderer/VulkanGraphicsContext.hpp"

#include <GLFW/glfw3.h>

#include <set>
#include <vector>

#include "LogPrivate.hpp"

namespace hexgon {

VulkanGraphicsContext::VulkanGraphicsContext(void* window) : GraphicsContext(), m_window(window) {}

VulkanGraphicsContext::~VulkanGraphicsContext() {}

void VulkanGraphicsContext::Init() {
  InitVkInstance();
  InitVkSurface();
  PickPhysicalDevice();
  CreateVkDevice();
}

void VulkanGraphicsContext::SwapBuffers() {}

void VulkanGraphicsContext::InitVkInstance() {
  VkApplicationInfo app_info{VK_STRUCTURE_TYPE_APPLICATION_INFO};
  app_info.pApplicationName = "Hexgon Engine";
  app_info.pEngineName = "Hexgon";
  app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  app_info.apiVersion = VK_API_VERSION_1_2;

  VkInstanceCreateInfo create_info{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
  create_info.pApplicationInfo = &app_info;

  uint32_t glfw_extension_count = 0;
  const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  create_info.enabledExtensionCount = glfw_extension_count;
  create_info.ppEnabledExtensionNames = glfw_extensions;

  VkResult ret = vkCreateInstance(&create_info, nullptr, &m_vk_instance);

  if (ret != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed create Vulkan instance");
    exit(-1);
  }

  HEX_CORE_INFO("Create VkInstance success");
}

void VulkanGraphicsContext::InitVkSurface() {
  if (glfwCreateWindowSurface(m_vk_instance, (GLFWwindow*)m_window, nullptr, &m_vk_surface) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed create window surface!");
    exit(-1);
  }
}

void VulkanGraphicsContext::PickPhysicalDevice() {
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(m_vk_instance, &device_count, nullptr);

  if (device_count == 0) {
    HEX_CORE_ERROR("No GPU support vulkan");
    exit(-1);
  }

  std::vector<VkPhysicalDevice> available_devices(device_count);

  vkEnumeratePhysicalDevices(m_vk_instance, &device_count, available_devices.data());

  int32_t graphic_queue_family = -1;
  int32_t present_queue_family = -1;

  for (size_t i = 0; i < available_devices.size(); i++) {
    uint32_t queue_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(available_devices[i], &queue_count, nullptr);

    std::vector<VkQueueFamilyProperties> properties(queue_count);

    vkGetPhysicalDeviceQueueFamilyProperties(available_devices[i], &queue_count, properties.data());

    for (size_t j = 0; j < properties.size(); j++) {
      if (properties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        graphic_queue_family = j;
      }

      VkBool32 present_support = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(available_devices[i], j, m_vk_surface, &present_support);

      if (present_support) {
        present_queue_family = j;
      }
    }

    if (graphic_queue_family >= 0 && present_queue_family >= 0) {
      m_phy_device = available_devices[i];
      m_graphic_queue_index = graphic_queue_family;
      m_present_queue_index = present_queue_family;
      break;
    } else {
      graphic_queue_family = -1;
      present_queue_family = -1;
    }
  }

  if (m_phy_device == 0) {
    HEX_CORE_ERROR("Failed to find a physical device");
    exit(-1);
  }

  vkGetPhysicalDeviceProperties(m_phy_device, &m_phy_props);

  HEX_CORE_INFO("Picked device name = {}", m_phy_props.deviceName);
}

void VulkanGraphicsContext::CreateVkDevice() {
  std::vector<VkDeviceQueueCreateInfo> queue_create_info{};

  std::set<int32_t> queue_families = {
      m_graphic_queue_index,
      m_present_queue_index,
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
    HEX_CORE_ERROR("Failed create logical device");
    exit(-1);
  }

  vkGetDeviceQueue(m_device, m_graphic_queue_index, 0, &m_graphic_queue);
  vkGetDeviceQueue(m_device, m_present_queue_index, 0, &m_present_queue);
}

}  // namespace hexgon

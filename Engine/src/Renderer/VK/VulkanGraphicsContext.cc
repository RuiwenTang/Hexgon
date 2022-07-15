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

#include "Renderer/VK/VulkanGraphicsContext.hpp"

#include <GLFW/glfw3.h>

#include <array>
#include <set>
#include <vector>

#include "LogPrivate.hpp"

#define HEX_ENABLE_VK_DEBUG 1

#ifdef HEX_ENABLE_VK_DEBUG
static bool g_enable_validation = true;
static const char* g_validation_name = "VK_LAYER_KHRONOS_validation";
#else
static bool g_enable_validation = false;
#endif

namespace hexgon {

#ifdef HEX_ENABLE_VK_DEBUG

static VkResult create_debug_utils_messenger_ext(VkInstance instance,
                                                 const VkDebugUtilsMessengerCreateInfoEXT* p_create_info,
                                                 const VkAllocationCallbacks* p_allocator,
                                                 VkDebugUtilsMessengerEXT* p_debug_messenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

  if (func != nullptr) {
    return func(instance, p_create_info, p_allocator, p_debug_messenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

static void destroy_debug_utils_messenger_ext(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger,
                                              const VkAllocationCallbacks* p_allocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

  if (func) {
    func(instance, debug_messenger, p_allocator);
  }
}

static bool check_validation_layer_support() {
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> layers{layer_count};
  vkEnumerateInstanceLayerProperties(&layer_count, layers.data());

  for (auto const& property : layers) {
    if (std::strcmp(property.layerName, g_validation_name) == 0) {
      return true;
    }
  }

  return false;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                     VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                     const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                     void* pUserData) {
  auto logger = spdlog::default_logger();
  auto level = spdlog::level::debug;

  if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    level = spdlog::level::err;
  } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    level = spdlog::level::warn;
  }

  logger->log(level, "{} Code {} : {}", pCallbackData->pMessageIdName, pCallbackData->messageIdNumber,
              pCallbackData->pMessage);
  if (0 < pCallbackData->queueLabelCount) {
    logger->log(level, "\t Queue Labels:");
    for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++) {
      logger->log(level, "\t\t labelName = [ {} ]", pCallbackData->pQueueLabels[i].pLabelName);
    }
  }

  if (0 < pCallbackData->cmdBufLabelCount) {
    logger->log(level, "\t CMD labels: \n");
    for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
      logger->log(level, "\t\t labelName = [ {} ]", pCallbackData->pCmdBufLabels[i].pLabelName);
    }
  }

  if (0 < pCallbackData->objectCount) {
    for (uint32_t i = 0; i < pCallbackData->objectCount; i++) {
      logger->log(level, "\t Object [{}]  \t\t objectType <{}> handle [{:X}] name : <{}>", i + 1,
                  pCallbackData->pObjects[i].objectType, pCallbackData->pObjects[i].objectHandle,
                  pCallbackData->pObjects[i].pObjectName ? pCallbackData->pObjects[i].pObjectName : "unknown");
    }
  }

  return VK_FALSE;
}

#endif

static VkFormat choose_swapchain_format(VkPhysicalDevice phy_device, VkSurfaceKHR surface) {
  uint32_t format_count = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(phy_device, surface, &format_count, nullptr);

  if (format_count == 0) {
    HEX_CORE_ERROR("failed get vulkan device format info");
    return VK_FORMAT_UNDEFINED;
  }

  std::vector<VkSurfaceFormatKHR> formats{format_count};
  vkGetPhysicalDeviceSurfaceFormatsKHR(phy_device, surface, &format_count, formats.data());

  if (format_count == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
    return VK_FORMAT_R8G8B8A8_UNORM;
  }

  for (auto format : formats) {
    if (format.format == VK_FORMAT_R8G8B8A8_UNORM && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
      return format.format;
    }
  }

  return formats[0].format;
}

static VkSampleCountFlagBits get_max_usable_sample_count(VkPhysicalDeviceProperties props) {
  VkSampleCountFlags counts = props.limits.framebufferColorSampleCounts & props.limits.framebufferDepthSampleCounts;

  if (counts & VK_SAMPLE_COUNT_64_BIT) return VK_SAMPLE_COUNT_64_BIT;
  if (counts & VK_SAMPLE_COUNT_32_BIT) return VK_SAMPLE_COUNT_32_BIT;
  if (counts & VK_SAMPLE_COUNT_16_BIT) return VK_SAMPLE_COUNT_16_BIT;
  if (counts & VK_SAMPLE_COUNT_8_BIT) return VK_SAMPLE_COUNT_8_BIT;
  if (counts & VK_SAMPLE_COUNT_4_BIT) return VK_SAMPLE_COUNT_4_BIT;
  if (counts & VK_SAMPLE_COUNT_2_BIT) return VK_SAMPLE_COUNT_2_BIT;

  return VK_SAMPLE_COUNT_1_BIT;
}

static bool get_support_depth_format(VkPhysicalDevice phy_device, VkFormat* out_format) {
  std::vector<VkFormat> depth_formats{
      VK_FORMAT_D32_SFLOAT_S8_UINT,
      VK_FORMAT_D24_UNORM_S8_UINT,
      VK_FORMAT_D16_UNORM_S8_UINT,
  };

  for (auto& format : depth_formats) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(phy_device, format, &props);
    if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
      *out_format = format;
      return true;
    }
  }

  return false;
}

VulkanGraphicsContext::VulkanGraphicsContext(void* window) : GraphicsContext(), m_window(window) {}

VulkanGraphicsContext::~VulkanGraphicsContext() {}

void VulkanGraphicsContext::Init() {
  InitVkInstance();
  InitVkSurface();
  PickPhysicalDevice();
  CreateVkDevice();
  CreateVkSwapchain();
  CreateSwapchainImageViews();
  CreateCommandPool();
  CreateCommandBuffer();
  CreateSyncObjects();
  CreateRenderPass();
  CreateFramebuffer();
}

void VulkanGraphicsContext::Destroy() {
  vkDestroyRenderPass(m_device, m_render_pass, nullptr);

  for (auto fb : m_framebuffers) {
    vkDestroyFramebuffer(m_device, fb, nullptr);
  }

  for (auto const& img : m_depth_images) {
    vkDestroyImageView(m_device, img.image_view, nullptr);
    vkDestroyImage(m_device, img.image, nullptr);
    vkFreeMemory(m_device, img.memory, nullptr);
  }

  for (auto const& img : m_sampler_images) {
    vkDestroyImageView(m_device, img.image_view, nullptr);
    vkDestroyImage(m_device, img.image, nullptr);
    vkFreeMemory(m_device, img.memory, nullptr);
  }

  for (auto sp : m_present_semaphore) {
    vkDestroySemaphore(m_device, sp, nullptr);
  }

  for (auto sp : m_render_semaphore) {
    vkDestroySemaphore(m_device, sp, nullptr);
  }

  for (auto fence : m_cmd_fences) {
    vkDestroyFence(m_device, fence, nullptr);
  }

  vkResetCommandPool(m_device, m_cmd_pool, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
  vkDestroyCommandPool(m_device, m_cmd_pool, nullptr);

  for (auto image_view : m_swapchain_views) {
    vkDestroyImageView(m_device, image_view, nullptr);
  }
  vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
  vkDestroyDevice(m_device, nullptr);
  vkDestroySurfaceKHR(m_vk_instance, m_vk_surface, nullptr);
#ifdef HEX_ENABLE_VK_DEBUG
  destroy_debug_utils_messenger_ext(m_vk_instance, m_debug_messenger, nullptr);
#endif
  vkDestroyInstance(m_vk_instance, nullptr);
}

void VulkanGraphicsContext::BeginFrame(const glm::vec4& clear_color) {
  VkResult result = vkAcquireNextImageKHR(m_device, m_swapchain, std::numeric_limits<uint64_t>::max(),
                                          m_present_semaphore[m_frame_index], VK_NULL_HANDLE, &m_current_frame);

  if (m_frame_index != m_current_frame) {
    HEX_CORE_ERROR("frame index is < {} > but current frame is < {} >", m_frame_index, m_current_frame);
  }

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    HEX_CORE_ERROR("need to handle window resize of recreate swap chain!");
    exit(-1);
  }

  VkCommandBuffer current_cmd = m_cmds[m_frame_index];

  vkResetCommandBuffer(current_cmd, 0);

  VkCommandBufferBeginInfo cmd_begin_info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
  cmd_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  if (vkBeginCommandBuffer(current_cmd, &cmd_begin_info) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed to begin cmd buffer at index : {}", m_current_frame);
    exit(-1);
  }

  std::vector<VkClearValue> clear_values{3};
  clear_values[0].color = {clear_color.x, clear_color.y, clear_color.z, clear_color.w};
  clear_values[1].depthStencil = {0.f, 0};
  clear_values[2].color = {clear_color.x, clear_color.y, clear_color.z, clear_color.w};

  VkRenderPassBeginInfo render_pass_begin_info{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
  render_pass_begin_info.renderPass = m_render_pass;
  render_pass_begin_info.framebuffer = m_framebuffers[m_current_frame];
  render_pass_begin_info.renderArea.offset = {0, 0};
  render_pass_begin_info.renderArea.extent = m_swapchain_extent;
  render_pass_begin_info.clearValueCount = clear_values.size();
  render_pass_begin_info.pClearValues = clear_values.data();

  vkCmdBeginRenderPass(current_cmd, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanGraphicsContext::SwapBuffers() {
  VkCommandBuffer current_cmd = m_cmds[m_frame_index];
  vkCmdEndRenderPass(current_cmd);

  if (vkEndCommandBuffer(current_cmd) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed to end cmd buffer at index : {}", m_frame_index);
    exit(-1);
  }

  VkPipelineStageFlags submit_pipeline_stages =
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

  VkSubmitInfo submit_info{VK_STRUCTURE_TYPE_SUBMIT_INFO};
  submit_info.pWaitDstStageMask = &submit_pipeline_stages;
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = &m_present_semaphore[m_frame_index];
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = &m_render_semaphore[m_frame_index];
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &current_cmd;

  vkResetFences(m_device, 1, &m_cmd_fences[m_frame_index]);

  if (vkQueueSubmit(m_present_queue, 1, &submit_info, m_cmd_fences[m_frame_index]) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed to submit command buffer!");
    exit(-1);
  }

  if (vkWaitForFences(m_device, 1, &m_cmd_fences[m_frame_index], VK_TRUE, std::numeric_limits<uint64_t>::max()) !=
      VK_SUCCESS) {
    HEX_CORE_ERROR("Error in wait fences");
    exit(-1);
  }

  VkPresentInfoKHR present_info{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &m_swapchain;
  present_info.pImageIndices = &m_current_frame;
  present_info.pWaitSemaphores = &m_render_semaphore[m_current_frame];
  present_info.waitSemaphoreCount = 1;

  VkResult result = vkQueuePresentKHR(m_present_queue, &present_info);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    HEX_CORE_ERROR("need to handle window resize of recreate swap chain!");
    exit(-1);
  }

  m_frame_index++;
  m_frame_index = m_frame_index % m_swapchain_views.size();
}

void VulkanGraphicsContext::InitVkInstance() {
  if (g_enable_validation && !check_validation_layer_support()) {
    HEX_CORE_ERROR("validation layers requested but not available!");
    exit(-1);
  }

  VkApplicationInfo app_info{VK_STRUCTURE_TYPE_APPLICATION_INFO};
  app_info.pApplicationName = "Hexgon Engine";
  app_info.pEngineName = "Hexgon";
  app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  app_info.apiVersion = VK_API_VERSION_1_2;

  VkInstanceCreateInfo create_info{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
  create_info.pApplicationInfo = &app_info;

  uint32_t glfw_extension_count = 0;
  const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  std::vector<const char*> extension_names{glfw_extension_count};
  for (uint32_t i = 0; i < glfw_extension_count; i++) {
    extension_names[i] = glfw_extensions[i];
  }

  if (g_enable_validation) {
    extension_names.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    create_info.enabledLayerCount = 1;
    create_info.ppEnabledLayerNames = &g_validation_name;
  }

  create_info.enabledExtensionCount = extension_names.size();
  create_info.ppEnabledExtensionNames = extension_names.data();

  VkResult ret = vkCreateInstance(&create_info, nullptr, &m_vk_instance);

  if (ret != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed create Vulkan instance");
    exit(-1);
  }

  if (g_enable_validation) {
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debug_create_info.messageSeverity =
        //   VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_create_info.pfnUserCallback = debug_callback;
#ifdef HEX_ENABLE_VK_DEBUG
    if (create_debug_utils_messenger_ext(m_vk_instance, &debug_create_info, nullptr, &m_debug_messenger) !=
        VK_SUCCESS) {
      HEX_CORE_ERROR("Failed to set up debug messenger!");
      exit(-1);
    }
#endif
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

      if (graphic_queue_family >= 0 && present_queue_family >= 0) {
        break;
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

void VulkanGraphicsContext::CreateVkSwapchain() {
  m_swapchain_format = choose_swapchain_format(m_phy_device, m_vk_surface);

  VkSurfaceCapabilitiesKHR surface_caps;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_phy_device, m_vk_surface, &surface_caps);

  m_swapchain_extent = surface_caps.currentExtent;

  VkCompositeAlphaFlagBitsKHR surface_composite;
  if (surface_caps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) {
    surface_composite = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  } else if (surface_caps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR) {
    surface_composite = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
  } else if (surface_caps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) {
    surface_composite = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
  } else {
    surface_composite = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
  }

  VkSwapchainCreateInfoKHR create_info{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
  create_info.surface = m_vk_surface;
  create_info.minImageCount = std::max(uint32_t(2), surface_caps.minImageCount);
  create_info.imageFormat = m_swapchain_format;
  create_info.imageExtent = surface_caps.currentExtent;
  create_info.imageArrayLayers = 1;
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  create_info.queueFamilyIndexCount = 1;
  create_info.pQueueFamilyIndices = (uint32_t*)&m_present_queue_index;
  create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  create_info.compositeAlpha = surface_composite;
  create_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
  create_info.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(m_device, &create_info, nullptr, &m_swapchain) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed to create vulkan swap chain");
    exit(-1);
  }
}

void VulkanGraphicsContext::CreateSwapchainImageViews() {
  uint32_t image_count = 0;
  vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, nullptr);

  std::vector<VkImage> images(image_count);

  vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, images.data());

  VkSampleCountFlagBits sample_count = get_max_usable_sample_count(m_phy_props);

  m_sampler_images.resize(image_count);
  for (size_t i = 0; i < m_sampler_images.size(); i++) {
    VkImageCreateInfo img_create_info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    img_create_info.imageType = VK_IMAGE_TYPE_2D;
    img_create_info.format = m_swapchain_format;
    img_create_info.extent = {m_swapchain_extent.width, m_swapchain_extent.height, 1};
    img_create_info.mipLevels = 1;
    img_create_info.arrayLayers = 1;
    img_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    img_create_info.samples = sample_count;
    img_create_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
    img_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if (vkCreateImage(m_device, &img_create_info, nullptr, &m_sampler_images[i].image) != VK_SUCCESS) {
      HEX_CORE_ERROR("Failed create swapchain image with MSAA");
      exit(-1);
    }

    VkMemoryRequirements mem_reqs{};
    vkGetImageMemoryRequirements(m_device, m_sampler_images[i].image, &mem_reqs);

    VkMemoryAllocateInfo mem_alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mem_alloc.allocationSize = mem_reqs.size;
    mem_alloc.memoryTypeIndex = GetMemroyType(mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(m_device, &mem_alloc, nullptr, &m_sampler_images[i].memory) != VK_SUCCESS) {
      HEX_CORE_ERROR("Failed allocate memory for swapchain image with MSAA");
      exit(-1);
    }

    if (vkBindImageMemory(m_device, m_sampler_images[i].image, m_sampler_images[i].memory, 0) != VK_SUCCESS) {
      HEX_CORE_ERROR("Faield bind memory for swapchain image");
      exit(-1);
    }

    VkImageViewCreateInfo view_create_info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    view_create_info.image = m_sampler_images[i].image;
    view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_create_info.format = m_swapchain_format;
    view_create_info.components.r = VK_COMPONENT_SWIZZLE_R;
    view_create_info.components.g = VK_COMPONENT_SWIZZLE_G;
    view_create_info.components.b = VK_COMPONENT_SWIZZLE_B;
    view_create_info.components.a = VK_COMPONENT_SWIZZLE_A;
    view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_create_info.subresourceRange.levelCount = 1;
    view_create_info.subresourceRange.layerCount = 1;

    if (vkCreateImageView(m_device, &view_create_info, nullptr, &m_sampler_images[i].image_view) != VK_SUCCESS) {
      HEX_CORE_ERROR("Failed create imageview for swapchain image");
      exit(-1);
    }

    m_sampler_images[i].format = m_swapchain_format;
  }

  m_swapchain_views.resize(image_count);
  for (size_t i = 0; i < m_swapchain_views.size(); i++) {
    VkImageViewCreateInfo create_info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.image = images[i];
    create_info.format = m_swapchain_format;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    if (vkCreateImageView(m_device, &create_info, nullptr, &m_swapchain_views[i]) != VK_SUCCESS) {
      HEX_CORE_ERROR("Failed to create swap chain image view");
      exit(-1);
    }
  }

  VkFormat depth_stencil_format;
  if (!get_support_depth_format(m_phy_device, &depth_stencil_format)) {
    HEX_CORE_ERROR("Failed find format support depth and stencil buffer");
    exit(-1);
  }

  m_depth_images.resize(image_count);

  VkImageCreateInfo depth_image_create_info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
  depth_image_create_info.imageType = VK_IMAGE_TYPE_2D;
  depth_image_create_info.format = depth_stencil_format;
  depth_image_create_info.extent = {m_swapchain_extent.width, m_swapchain_extent.height, 1};
  depth_image_create_info.mipLevels = 1;
  depth_image_create_info.arrayLayers = 1;
  depth_image_create_info.samples = sample_count;
  depth_image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
  depth_image_create_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

  for (size_t i = 0; i < m_depth_images.size(); i++) {
    if (vkCreateImage(m_device, &depth_image_create_info, nullptr, &m_depth_images[i].image) != VK_SUCCESS) {
      HEX_CORE_ERROR("Failed create depth stencil buffer");
      exit(-1);
    }

    VkMemoryRequirements mem_reqs{};
    vkGetImageMemoryRequirements(m_device, m_depth_images[i].image, &mem_reqs);

    VkMemoryAllocateInfo mem_alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    mem_alloc.allocationSize = mem_reqs.size;
    mem_alloc.memoryTypeIndex = GetMemroyType(mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(m_device, &mem_alloc, nullptr, &m_depth_images[i].memory) != VK_SUCCESS) {
      HEX_CORE_ERROR("Failed to allocate depth stencil buffer memory");
      exit(-1);
    }

    if (vkBindImageMemory(m_device, m_depth_images[i].image, m_depth_images[i].memory, 0) != VK_SUCCESS) {
      HEX_CORE_ERROR("Failed to bind depth stencil buffer memory");
      exit(-1);
    }

    VkImageViewCreateInfo image_view_create_info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    image_view_create_info.image = m_depth_images[i].image;
    image_view_create_info.format = depth_stencil_format;
    image_view_create_info.subresourceRange.baseMipLevel = 0;
    image_view_create_info.subresourceRange.levelCount = 1;
    image_view_create_info.subresourceRange.baseArrayLayer = 0;
    image_view_create_info.subresourceRange.layerCount = 1;
    image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

    if (vkCreateImageView(m_device, &image_view_create_info, nullptr, &m_depth_images[i].image_view) != VK_SUCCESS) {
      HEX_CORE_ERROR("Failed to create image view for depth stencil buffer");
      exit(-1);
    }

    m_depth_images[i].format = depth_stencil_format;
  }
  m_sample_count = sample_count;
  HEX_CORE_INFO("Finish create all swapchain images for [ {} ] buffers", image_count);
}

void VulkanGraphicsContext::CreateCommandPool() {
  VkCommandPoolCreateInfo create_info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
  create_info.queueFamilyIndex = m_graphic_queue_index;
  create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  if (vkCreateCommandPool(m_device, &create_info, nullptr, &m_cmd_pool) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed create command pool");
    exit(-1);
  }
}

void VulkanGraphicsContext::CreateCommandBuffer() {
  m_cmds.resize(m_swapchain_views.size());

  VkCommandBufferAllocateInfo allocate_info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
  allocate_info.commandPool = m_cmd_pool;
  allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocate_info.commandBufferCount = static_cast<uint32_t>(m_cmds.size());

  if (vkAllocateCommandBuffers(m_device, &allocate_info, m_cmds.data()) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed to allocate command buffers");
    exit(-1);
  }

  HEX_CORE_INFO("Creat [ {} ] command buffers", m_cmds.size());
}

void VulkanGraphicsContext::CreateSyncObjects() {
  VkFenceCreateInfo create_info{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
  create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  m_cmd_fences.resize(m_cmds.size());

  for (size_t i = 0; i < m_cmd_fences.size(); i++) {
    if (vkCreateFence(m_device, &create_info, nullptr, &m_cmd_fences[i]) != VK_SUCCESS) {
      HEX_CORE_ERROR("Failed to create fence at index : {}", i);
      exit(-1);
    }
  }

  m_present_semaphore.resize(m_cmds.size());
  m_render_semaphore.resize(m_cmds.size());

  VkSemaphoreCreateInfo semaphore_create_info{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

  for (size_t i = 0; i < m_present_semaphore.size(); i++) {
    if (vkCreateSemaphore(m_device, &semaphore_create_info, nullptr, &m_present_semaphore[i]) != VK_SUCCESS) {
      HEX_CORE_ERROR("Failed to create present semaphore");
      exit(-1);
    }
  }
  for (size_t i = 0; i < m_render_semaphore.size(); i++) {
    if (vkCreateSemaphore(m_device, &semaphore_create_info, nullptr, &m_render_semaphore[i]) != VK_SUCCESS) {
      HEX_CORE_ERROR("Failed to create render semaphore");
      exit(-1);
    }
  }
}

void VulkanGraphicsContext::CreateRenderPass() {
  std::array<VkAttachmentDescription, 3> attachments = {};
  // color attachment
  attachments[0].format = m_swapchain_format;
  attachments[0].samples = m_sample_count;
  attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  // depth stencil attachment
  attachments[1].format = m_depth_images[0].format;
  attachments[1].samples = m_sample_count;
  attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  // color resolve attachment
  attachments[2].format = m_swapchain_format;
  attachments[2].samples = VK_SAMPLE_COUNT_1_BIT;
  attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attachments[2].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_reference{};
  color_reference.attachment = 0;
  color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depth_stencil_reference{};
  depth_stencil_reference.attachment = 1;
  depth_stencil_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference resolve_reference{};
  resolve_reference.attachment = 2;
  resolve_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_reference;
  subpass.pDepthStencilAttachment = &depth_stencil_reference;
  subpass.pResolveAttachments = &resolve_reference;

  VkSubpassDependency subpass_deps{};
  subpass_deps.srcSubpass = VK_SUBPASS_EXTERNAL;
  subpass_deps.dstSubpass = 0;
  subpass_deps.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
  subpass_deps.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subpass_deps.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
  subpass_deps.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  subpass_deps.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

  VkRenderPassCreateInfo create_info{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
  create_info.attachmentCount = static_cast<uint32_t>(attachments.size());
  create_info.pAttachments = attachments.data();
  create_info.subpassCount = 1;
  create_info.pSubpasses = &subpass;
  create_info.dependencyCount = 0;

  if (vkCreateRenderPass(m_device, &create_info, nullptr, &m_render_pass) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed to create render pass!");
    exit(-1);
  }
}

void VulkanGraphicsContext::CreateFramebuffer() {
  m_framebuffers.resize(m_swapchain_views.size());

  for (size_t i = 0; i < m_framebuffers.size(); i++) {
    std::array<VkImageView, 3> attachments = {
        m_sampler_images[i].image_view,
        m_depth_images[i].image_view,
        m_swapchain_views[i],
    };

    VkFramebufferCreateInfo create_info{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    create_info.renderPass = m_render_pass;
    create_info.attachmentCount = attachments.size();
    create_info.pAttachments = attachments.data();
    create_info.width = m_swapchain_extent.width;
    create_info.height = m_swapchain_extent.height;
    create_info.layers = 1;

    if (vkCreateFramebuffer(m_device, &create_info, nullptr, &m_framebuffers[i]) != VK_SUCCESS) {
      HEX_CORE_ERROR("Failed to create frame buffer");
      exit(-1);
    }
  }
}

uint32_t VulkanGraphicsContext::GetMemroyType(uint32_t type_bits, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memory_props;
  vkGetPhysicalDeviceMemoryProperties(m_phy_device, &memory_props);

  for (uint32_t i = 0; i < memory_props.memoryTypeCount; i++) {
    if ((type_bits & 1) == 1) {
      if ((memory_props.memoryTypes[i].propertyFlags & properties) == properties) {
        return i;
      }
    }

    type_bits >>= 1;
  }

  return 0;
}

}  // namespace hexgon

#include "src/render/vulkan/render_system_vk.hpp"

#include <hexgon/core/log.hpp>

namespace Hexgon {

static const char* g_validation_name = "VK_LAYER_KHRONOS_validation";

static VkResult create_debug_utils_messenger_ext(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* p_create_info,
    const VkAllocationCallbacks* p_allocator,
    VkDebugUtilsMessengerEXT* p_debug_messenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");

  if (func != nullptr) {
    return func(instance, p_create_info, p_allocator, p_debug_messenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

static void destroy_debug_utils_messenger_ext(
    VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger,
    const VkAllocationCallbacks* p_allocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");

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

static VKAPI_ATTR VkBool32 VKAPI_CALL
debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
               VkDebugUtilsMessageTypeFlagsEXT messageType,
               const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
               void* pUserData) {
  HEX_CORE_INFO("{} Code {} : {}", pCallbackData->pMessageIdName,
                pCallbackData->messageIdNumber, pCallbackData->pMessage);
  if (0 < pCallbackData->queueLabelCount) {
    HEX_CORE_INFO("\t Queue Labels:");
    for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++) {
      HEX_CORE_INFO("\t\t labelName = [ {} ]",
                    pCallbackData->pQueueLabels[i].pLabelName);
    }
  }

  if (0 < pCallbackData->cmdBufLabelCount) {
    HEX_CORE_INFO("\t CMD labels: \n");
    for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
      HEX_CORE_INFO("\t\t labelName = [ {} ]",
                    pCallbackData->pCmdBufLabels[i].pLabelName);
    }
  }

  if (0 < pCallbackData->objectCount) {
    for (uint32_t i = 0; i < pCallbackData->objectCount; i++) {
      HEX_CORE_INFO(
          "\t Object [{}]  \t\t objectType <{}> handle [{:X}] name : <{}>",
          i + 1, pCallbackData->pObjects[i].objectType,
          pCallbackData->pObjects[i].objectHandle,
          pCallbackData->pObjects[i].pObjectName
              ? pCallbackData->pObjects[i].pObjectName
              : "unknown");
    }
  }

  return VK_FALSE;
}

Scope<RenderSystem> LoadVulkanRender() { return CreateScope<RenderSystemVK>(); }

bool RenderSystemVK::Init() {
  if (!InitVkInstance()) {
    return false;
  }

  if (!PickDevice()) {
    return false;
  }

  return true;
}

void RenderSystemVK::Shutdown() { vkDestroyInstance(m_vk_ins, nullptr); }

Scope<Swapchain> RenderSystemVK::CreateSwapchain(
    Window* window, const SwapchainDescriptor& desc, RenderPass* renderPass) {
  return Scope<Swapchain>();
}

Scope<RenderPass> RenderSystemVK::CreateRenderPass(
    const RenderPassDescriptor& desc) {
  return Scope<RenderPass>();
}

Ref<Texture> RenderSystemVK::CreateTexture(const TextureDescriptor& desc) {
  return Ref<Texture>();
}

bool RenderSystemVK::InitVkInstance() {
  bool has_validation = check_validation_layer_support();

  // create vulkan instance
  VkApplicationInfo app_info{VK_STRUCTURE_TYPE_APPLICATION_INFO};
  app_info.pApplicationName = "Hexgon Engine";
  app_info.pEngineName = "Hexgon";
  app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  app_info.apiVersion = VK_API_VERSION_1_3;

  VkInstanceCreateInfo create_info{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};

  create_info.pApplicationInfo = &app_info;

  uint32_t glfw_extension_count = 0;
  const char** glfw_extensions =
      glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  std::vector<const char*> extension_names{glfw_extension_count};
  for (uint32_t i = 0; i < glfw_extension_count; i++) {
    extension_names[i] = glfw_extensions[i];
  }

  // TODO disable validation by config
  extension_names.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  create_info.enabledExtensionCount = extension_names.size();
  create_info.ppEnabledExtensionNames = extension_names.data();

  VkResult ret = vkCreateInstance(&create_info, nullptr, &m_vk_ins);

  if (ret != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed create vulkan instance");
    return false;
  }

  VkDebugUtilsMessengerCreateInfoEXT debug_create_info{
      VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
  debug_create_info.messageSeverity =
      //   VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  debug_create_info.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  debug_create_info.pfnUserCallback = debug_callback;

  if (create_debug_utils_messenger_ext(m_vk_ins, &debug_create_info, nullptr,
                                       &m_vk_debug_msg) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed to set up debug messenger!");
    return false;
  }

  return true;
}

bool RenderSystemVK::PickDevice() {
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(m_vk_ins, &device_count, nullptr);

  if (device_count == 0) {
    HEX_CORE_ERROR("No GPU support vulkan");
    return false;
  }

  std::vector<VkPhysicalDevice> available_devices(device_count);

  vkEnumeratePhysicalDevices(m_vk_ins, &device_count, available_devices.data());

  int32_t graphic_queue_family = -1;
  int32_t present_queue_family = -1;

  if (available_devices.size() == 1) {
    m_phy_dev = available_devices[0];
  } else {
    for (size_t i = 0; i < available_devices.size(); i++) {
      
    }
  }

  VkPhysicalDeviceProperties props;
  vkGetPhysicalDeviceProperties(m_phy_dev, &props);

  HEX_CORE_INFO("Pick GPU: {}", props.deviceName);

  return true;
}

}  // namespace Hexgon

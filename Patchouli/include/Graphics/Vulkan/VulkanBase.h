#pragma once

#ifdef PATCHOULI_DEBUG_MODE
constexpr const char* VULKAN_VALIDATION_LAYER = "VK_LAYER_KHRONOS_validation";
#endif

// Vulkan Debug Messenger
constexpr const char* CREATE_DEBUG_UTILS_MESSENGER = "vkCreateDebugUtilsMessengerEXT";
constexpr const char* DESTROY_DEBUG_UTILS_MESSENGER = "vkDestroyDebugUtilsMessengerEXT";
#pragma once

#ifdef PATCHOULI_DEBUG_MODE
#	define PATCHOULI_VULKAN_VALIDATION_LAYER "VK_LAYER_KHRONOS_validation"
#endif

// Vulkan Debug Messenger
#define PATCHOULI_VULKAN_CREATE_DEBUG_UTILS_MESSENGER "vkCreateDebugUtilsMessengerEXT"
#define PATCHOULI_VULKAN_DESTROY_DEBUG_UTILS_MESSENGER "vkDestroyDebugUtilsMessengerEXT"
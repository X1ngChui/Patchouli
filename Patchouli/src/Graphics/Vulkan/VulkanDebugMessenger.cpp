#include "Graphics/Vulkan/VulkanDebugMessenger.h"
#include "Log/Console.h"

namespace Patchouli
{
    // Constructor for VulkanDebugMessenger.
    // It creates a Vulkan debug messenger associated with the provided VulkanInstance and VulkanAllocator.
    VulkanDebugMessenger::VulkanDebugMessenger(Ref<VulkanInstance> instance, Ref<VulkanAllocator> allocator)
        : vkInstance(instance), vkAllocator(allocator)
    {
        // Configure Vulkan debug messenger creation info
        VkDebugUtilsMessengerCreateInfoEXT info = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = nullptr,
            .flags = 0,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = &console, // Specify the static callback function for handling debug messages
            .pUserData = nullptr,
        };

        VkResult result = VK_SUCCESS;
        VkAllocationCallbacks allocationCallbacks = *vkAllocator.lock();

        // Retrieve function pointer for vkCreateDebugUtilsMessengerEXT
        auto createDebugMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*vkInstance.lock(), "vkCreateDebugUtilsMessengerEXT");

        if (createDebugMessenger == nullptr)
        {
            result = VK_ERROR_EXTENSION_NOT_PRESENT;
            goto error;
        }

        // Create Vulkan debug messenger
        result = createDebugMessenger(*vkInstance.lock(), &info, &allocationCallbacks, &debugMessenger);

    error:
        // Assert if Vulkan debug messenger creation fails
        Console::coreAssert(result == VK_SUCCESS, "Vulkan Debug Messenger creation failed.");
    }

    // Destructor for VulkanDebugMessenger.
    // It cleans up Vulkan debug messenger resources.
    VulkanDebugMessenger::~VulkanDebugMessenger()
    {
        VkResult result = VK_SUCCESS;
        VkAllocationCallbacks allocationCallbacks = *vkAllocator.lock();

        // Retrieve function pointer for vkDestroyDebugUtilsMessengerEXT
        auto destroyDebugMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*vkInstance.lock(), "vkDestroyDebugUtilsMessengerEXT");

        if (destroyDebugMessenger == nullptr)
        {
            result = VK_ERROR_EXTENSION_NOT_PRESENT;
            // Assert if function pointer retrieval fails
            Console::coreAssert(false, "Vulkan Debug Messenger destruction function loading failed.");
        }

        // Destroy Vulkan debug messenger
        destroyDebugMessenger(*vkInstance.lock(), debugMessenger, &allocationCallbacks);
    }

    // Member function for handling Vulkan debug messages.
    // It logs the debug message to the console.
    void VulkanDebugMessenger::consoleImpl(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* message)
    {
        switch (severity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            Console::coreTrace("Vulkan Trace: {}", message->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            Console::coreInfo("Vulkan Info: {}", message->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            Console::coreWarn("Vulkan Warn: {}", message->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            Console::coreError("Vulkan Error: {}", message->pMessage);
            break;
        }
    }
}

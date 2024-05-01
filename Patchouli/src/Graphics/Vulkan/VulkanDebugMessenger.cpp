#include "Graphics/Vulkan/VulkanDebugMessenger.h"
#include "Graphics/Vulkan/VulkanBase.h"
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
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
            .pfnUserCallback = &VulkanDebugMessenger::console, // Specify the static callback function for handling debug messages
            .pUserData = this,
        };

        // Retrieve function pointer for vkCreateDebugUtilsMessengerEXT
        auto createDebugMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*vkInstance, PATCHOULI_VULKAN_CREATE_DEBUG_UTILS_MESSENGER);

        assert(createDebugMessenger != nullptr);

        // Create Vulkan debug messenger
        VkResult result = createDebugMessenger(*vkInstance, &info, *vkAllocator, &debugMessenger);

        // Assert if Vulkan debug messenger creation fails
        assert(result == VK_SUCCESS);
    }

    // Destructor for VulkanDebugMessenger.
    // It cleans up Vulkan debug messenger resources.
    VulkanDebugMessenger::~VulkanDebugMessenger()
    {
        // Retrieve function pointer for vkDestroyDebugUtilsMessengerEXT
        auto destroyDebugMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*vkInstance, PATCHOULI_VULKAN_DESTROY_DEBUG_UTILS_MESSENGER);

        // Assert if function pointer retrieval fails
        assert(destroyDebugMessenger != nullptr);

        // Destroy Vulkan debug messenger
        destroyDebugMessenger(*vkInstance, debugMessenger, *vkAllocator);
    }

    // Member function for handling Vulkan debug messages.
    // It logs the debug message to the console.
    void VulkanDebugMessenger::consoleImpl(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* message)
    {
        (void)messageType;
        switch (severity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            Console::coreTrace("Vulkan Trace: {}", message->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            Console::coreInfo("Vulkan Info: {}", message->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            Console::coreWarn("Vulkan Warning: {}", message->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            Console::coreError("Vulkan Error: {}", message->pMessage);
            break;
        }
    }
}

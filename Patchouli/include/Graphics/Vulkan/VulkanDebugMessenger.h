#pragma once

#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
    // VulkanDebugMessenger class manages Vulkan debug messaging.
    class VulkanDebugMessenger : public RefBase<VulkanDebugMessenger>
    {
    public:
        // Constructor for VulkanDebugMessenger.
        // It creates a Vulkan debug messenger associated with the provided VulkanInstance and VulkanAllocator.
        VulkanDebugMessenger(Ref<VulkanInstance> instance, Ref<VulkanAllocator> allocator);

        // Destructor for VulkanDebugMessenger.
        // It cleans up Vulkan debug messenger resources.
        ~VulkanDebugMessenger();

        // Static callback function for handling Vulkan debug messages.
        // It redirects the message to the consoleImpl member function of the associated VulkanDebugMessenger object.
        static VKAPI_ATTR VkBool32 VKAPI_CALL console(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* message, void* pUserData)
        {
            static_cast<VulkanDebugMessenger*>(pUserData)->consoleImpl(severity, messageType, message);
            return VK_FALSE;
        }

        // Member function for handling Vulkan debug messages.
        // It logs the debug message to the console.
        void consoleImpl(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* message);

    private:
        Ref<VulkanInstance> instance = nullptr;  // Vulkan instance
        Ref<VulkanAllocator> allocator = nullptr; // Vulkan allocator
        VkDebugUtilsMessengerEXT vkDebugMessenger; // Vulkan debug messenger handle
    };
}

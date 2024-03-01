#include "Graphics/Vulkan/VulkanContext.h"
#include "Log/Console.h"
#include <GLFW/glfw3.h>

namespace Patchouli
{
    // Constructor for VulkanContext.
    // It initializes the Vulkan allocator and instance based on the provided GraphicsInfo object.
    VulkanContext::VulkanContext(const GraphicsInfo& info)
    {
        // Initialize Vulkan allocator
        vkAllocator = makeRef<VulkanAllocator>();

        // Create Vulkan instance using the provided GraphicsInfo object and Vulkan allocator
        vkInstance = makeRef<VulkanInstance>(info, vkAllocator);

#ifdef PATCHOULI_VULKAN_VALIDATION
        // Create Vulkan debug messenger for validation purposes, if enabled
        vkDebugMessenger = makeRef<VulkanDebugMessenger>(vkInstance, vkAllocator);
#endif
    }

    std::vector<Ref<GraphicsDevice>> VulkanContext::getDevices() const
    {
        return VulkanDevice::getDevices(vkInstance);
    }

    void VulkanContext::selectDevice(Ref<GraphicsDevice> device)
    {
        vkDevice = std::dynamic_pointer_cast<VulkanDevice>(device);
    }
}
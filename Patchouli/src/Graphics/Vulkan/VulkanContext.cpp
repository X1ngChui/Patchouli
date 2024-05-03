#include "Graphics/Vulkan/VulkanContext.h"
#include <GLFW/glfw3.h>

namespace Patchouli
{
    // Constructor for VulkanContext.
    // It initializes the Vulkan allocator and instance based on the provided GraphicsInfo object.
    VulkanContext::VulkanContext(const GraphicsContextCreateInfo& info)
        : graphicsContextInfo(info)
    {
        // Initialize Vulkan allocator
        vkAllocator = makeRef<VulkanAllocator>();

        // Create Vulkan instance using the provided GraphicsInfo object and Vulkan allocator
        vkInstance = makeRef<VulkanInstance>(vkAllocator, graphicsContextInfo);

#ifdef PATCHOULI_VULKAN_VALIDATION
        // Create Vulkan debug messenger for validation purposes, if enabled
        vkDebugMessenger = makeRef<VulkanDebugMessenger>(vkInstance, vkAllocator);
#endif
        // Create Vulkan surface for displaying
        vkSurface = makeRef<VulkanSurface>(vkInstance, vkAllocator, graphicsContextInfo.window);

        // Select and create graphics Device
        auto vkDevices = VulkanDevice::getDevices(vkInstance);
        auto devices = std::vector<Ref<GraphicsDevice>>(vkDevices.begin(), vkDevices.end());
        vkDevice = std::static_pointer_cast<VulkanDevice>(graphicsContextInfo.deviceSelector(devices));
        vkDevice->onSelect(vkAllocator, vkSurface, graphicsContextInfo);

        vkSwapchain = makeRef<VulkanSwapchain>(graphicsContextInfo, vkDevice, vkSurface, vkAllocator);

        vkPipeline = makeRef<VulkanPipeline>(vkDevice, vkAllocator);
    }
}
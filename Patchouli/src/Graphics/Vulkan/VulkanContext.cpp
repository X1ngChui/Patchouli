#include "Graphics/Vulkan/VulkanContext.h"
#include <GLFW/glfw3.h>

namespace Patchouli
{
    // Constructor for VulkanContext.
    // It initializes the Vulkan allocator and instance based on the provided GraphicsInfo object.
    VulkanContext::VulkanContext(const GraphicsContextCreateInfo& info)
        : graphicsInfo(info)
    {
        // Initialize Vulkan allocator
        vkAllocator = makeRef<VulkanAllocator>();

        // Create Vulkan instance using the provided GraphicsInfo object and Vulkan allocator
        vkInstance = makeRef<VulkanInstance>(vkAllocator, info);

#ifdef PATCHOULI_VULKAN_VALIDATION
        // Create Vulkan debug messenger for validation purposes, if enabled
        vkDebugMessenger = makeRef<VulkanDebugMessenger>(vkInstance, vkAllocator);
#endif

        vkSurface = makeRef<VulkanSurface>(vkInstance, vkAllocator, info.window);
    }

    std::vector<Ref<GraphicsDevice>> VulkanContext::getDevices() const
    {
        return VulkanDevice::getDevices(vkInstance);
    }

    void VulkanContext::selectDevice(Ref<GraphicsDevice> device)
    {
        vkDevice = std::static_pointer_cast<VulkanDevice>(device);
        vkDevice->onSelect(vkAllocator, vkSurface, graphicsInfo);

        if (*vkSurface)
            vkSwapchain = makeRef<VulkanSwapchain>(graphicsInfo, vkDevice, vkSurface, vkAllocator);

        // vkPipeline = makeRef<VulkanPipeline>(vkDevice, vkAllocator);
    }
}
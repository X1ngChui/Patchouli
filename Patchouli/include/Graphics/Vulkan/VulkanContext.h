#pragma once
#include "PatchouliPch.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/Vulkan/VulkanBase.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Graphics/Vulkan/VulkanDebugMessenger.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include "Graphics/Vulkan/VulkanSwapchain.h"
#include "Graphics/Vulkan/VulkanPipeline.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
    // VulkanContext class responsible for managing Vulkan graphics context.
    class VulkanContext : public GraphicsContext
    {
    public:
        // Constructor for VulkanContext.
        VulkanContext(const GraphicsContextCreateInfo& info);

        // Destructor for VulkanContext.
        virtual ~VulkanContext() = default;

    private:
        // Graphics settings info.
        GraphicsContextCreateInfo graphicsContextInfo;

        // Vulkan memory allocator.
        // Note: vkAllocator must be placed as the first member
        // to ensure correct destruction order specified by the standard.
        Ref<VulkanAllocator> vkAllocator = nullptr;

        // Vulkan instance.
        // Note: vkInstance must be placed as the second member
        // to ensure correct destruction order specified by the standard.
        Ref<VulkanInstance> vkInstance = nullptr;

#ifdef PATCHOULI_VULKAN_VALIDATION
        // Vulkan debug messenger (optional, enabled by preprocessor macro).
        Ref<VulkanDebugMessenger> vkDebugMessenger = nullptr;
#endif

        Ref<VulkanSurface> vkSurface = nullptr; // Vulkan surface to display image

        Ref<VulkanDevice> vkDevice = nullptr; // Vulkan device used by the context

        Ref<VulkanSwapchain> vkSwapchain = nullptr; // Vulkan sawpchain to display image

        Ref<VulkanPipeline> vkPipeline = nullptr; // Vulkan pipeline for rendering
    };
}

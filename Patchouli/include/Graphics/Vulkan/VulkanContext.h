#pragma once
#include "Graphics/GraphicsContext.h"
#include "Graphics/Vulkan/VulkanBase.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Graphics/Vulkan/VulkanDebugMessenger.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include "Graphics/Vulkan/VulkanSwapchain.h"
#include "Graphics/Vulkan/VulkanRenderPass.h"
#include "Graphics/Vulkan/VulkanPipeline.h"
#include "Graphics/Vulkan/VulkanCommandPool.h"
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
        Ref<VulkanAllocator> allocator = nullptr;

        // Vulkan instance.
        Ref<VulkanInstance> instance = nullptr;

#ifdef PATCHOULI_VULKAN_VALIDATION_LAYER
        // Vulkan debug messenger (optional, enabled by preprocessor macro).
        Ref<VulkanDebugMessenger> debugMessenger = nullptr;
#endif

        Ref<VulkanSurface> surface = nullptr; // Vulkan surface to display image

        Ref<VulkanDevice> device = nullptr; // Vulkan device used by the context

        Ref<VulkanSwapchain> swapchain = nullptr; // Vulkan sawpchain to display image

        Ref<VulkanRenderPass> renderPass = nullptr;

        Ref<VulkanPipeline> pipeline = nullptr; // Vulkan pipeline for rendering

        Ref<VulkanCommandPool> commandPool = nullptr;

        std::vector<Ref<VulkanCommandBuffer>> commandBuffers;
    };
}

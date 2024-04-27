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
        // Note: According to the C++ standard (ISO/IEC 14882:2011, ¡ì12.4.10),
        // member variables are destructed in the reverse order of their declaration.
        // Therefore, vkAllocator, being the last member declared, is destructed last.
        virtual ~VulkanContext() = default;

        // Function to retrieve a list of Vulkan graphics devices.
        virtual std::vector<Ref<GraphicsDevice>> getDevices() const override;

        // Function to select a specific Vulkan graphics device.
        virtual void selectDevice(Ref<GraphicsDevice> device) override;

    private:
        // Graphics settings info.
        GraphicsContextCreateInfo graphicsInfo;

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

        Ref<VulkanSwapchain> vkSwapchain = nullptr;
    };
}

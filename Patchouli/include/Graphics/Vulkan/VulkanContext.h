#pragma once
#include "Graphics/GraphicsContext.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Graphics/Vulkan/VulkanDebugMessenger.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
    // VulkanContext class responsible for managing Vulkan graphics context.
    class VulkanContext : public GraphicsContext
    {
    public:
        // Constructor accepting GraphicsInfo object.
        VulkanContext(const GraphicsInfo& info);

        // Destructor for VulkanContext.
        // Note: According to the C++ standard (ISO/IEC 14882:2011, ¡ì12.4.10),
        // member variables are destructed in the reverse order of their declaration.
        // Therefore, vkAllocator, being the last member declared, is destructed last.
        virtual ~VulkanContext() = default;

    private:
        // Vulkan memory allocator.
        // Note: vkAllocator must be placed as the first member
        // to ensure correct destruction order specified by the standard.
        Ref<VulkanAllocator> vkAllocator;

        // Vulkan instance.
        // Note: vkAllocator must be placed as the second member
        // to ensure correct destruction order specified by the standard.
        Ref<VulkanInstance> vkInstance;

#ifdef PATCHOULI_VULKAN_VALIDATION
        // Vulkan debug messenger (optional, enabled by preprocessor macro).
        Ref<VulkanDebugMessenger> vkDebugMessenger;
#endif
    };
}

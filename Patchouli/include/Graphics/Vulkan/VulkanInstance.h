#pragma once

#include "Util/Util.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include <vulkan/vulkan.h>

// Conditionally define PATCHOULI_VULKAN_VALIDATION macro based on PATCHOULI_DEBUG_MODE
#ifdef PATCHOULI_DEBUG_MODE
#define PATCHOULI_VULKAN_VALIDATION
#endif

namespace Patchouli
{
    // VulkanInstance class manages Vulkan instance creation and destruction.
    class VulkanInstance : public RefBase<VulkanInstance>
    {
    public:
        // Constructor for VulkanInstance.
        // It creates a Vulkan instance using the provided GraphicsInfo and VulkanAllocator.
        VulkanInstance(const GraphicsInfo& info, Ref<VulkanAllocator> allocator);

        // Destructor for VulkanInstance.
        // It cleans up Vulkan instance resources.
        ~VulkanInstance();

        // Conversion operator to VkInstance.
        // It allows VulkanInstance objects to be implicitly converted to VkInstance.
        inline operator VkInstance() const { return vkInstance; }

    private:
        // Private member function to retrieve required Vulkan extensions based on WindowAPI.
        std::vector<const char*> getExtensions(WindowAPI windowAPI) const;

        // Private member function to check if specified Vulkan layers are supported.
        bool checkLayers(std::vector<const char*>& layers) const;

    private:
        VkInstance vkInstance;                      // Vulkan instance handle
        WeakRef<VulkanAllocator> vkAllocator;       // Weak reference to Vulkan allocator for memory management
    };
}

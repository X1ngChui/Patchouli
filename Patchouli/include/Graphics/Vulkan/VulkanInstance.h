#pragma once

#include "PatchouliPch.h"
#include "Core/PObject.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
    // VulkanInstance class manages Vulkan instance creation and destruction.
    class VulkanInstance : public PObject
    {
    public:
        // Constructor for VulkanInstance.
        // It creates a Vulkan instance using the VulkanAllocator.
        VulkanInstance(Ref<VulkanAllocator> allocator, const GraphicsContextCreateInfo& info);

        // Destructor for VulkanInstance.
        // It cleans up Vulkan instance resources.
        ~VulkanInstance();

        // Conversion operator to VkInstance.
        // It allows VulkanInstance objects to be implicitly converted to VkInstance.
        inline operator VkInstance() const { return vkInstance; }

    private:
        // Private member function to retrieve required Vulkan extensions based on WindowAPI.
        VulkanVector<const char*> getEnabledExtensions(WindowAPI windowAPI) const;

        // Private member function to retrieve required Vulkan layers.
        VulkanVector<const char*> getEnabledLayers() const;

        // Private member function to check if specified Vulkan layers are supported.
        bool checkLayers(VulkanVector<const char*>& layers) const;

    private:
        VkInstance vkInstance; // Vulkan instance handle
        Ref<VulkanAllocator> vkAllocator = nullptr; // Vulkan allocator
    };
}

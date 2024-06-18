#pragma once

#include "Graphics/GraphicsDevice.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
    enum VulkanQueueFamilyIndex : uint32_t
    {
        None = 0xffffffff
    };

    // Represents a Vulkan-compatible graphics device.
    class VulkanDevice : public GraphicsDevice
    {
    public:
        // Constructor. Initializes with the specified Vulkan physical device.
        VulkanDevice(VkPhysicalDevice physicalDevice);

        // Destructor.
        virtual ~VulkanDevice();

        // Retrieves device properties.
        virtual GraphicsDeviceProperties getProperties() const override;

        // Retrieves device features.
        virtual GraphicsDeviceFeatures getFeatures() const override;

        // Selects the Vulkan device and initializes its properties.
        void onSelect(Ref<VulkanAllocator> allocator, Ref<VulkanSurface> surface, const GraphicsContextCreateInfo& info);

        // Retrieves a list of Vulkan devices associated with a Vulkan instance.
        static std::vector<Ref<GraphicsDevice>> getDevices(Ref<VulkanInstance> instance);

        // Conversion operator to VkDevice.
        // It allows VulkanDevice objects to be implicitly converted to VkDevice.
        inline operator VkDevice() const { return vkDevice; }

        // Conversion operator to VkInstance.
        // It allows VulkanDevice objects to be implicitly converted to VkPhysicalDevice.
        inline operator VkPhysicalDevice() const { return vkPhysicalDevice; }

        // Getter function for graphics queue family indices.
        VulkanQueueFamilyIndex getGraphicsQueueFamilyIndex() const { return graphicsQueueFamilyIndex; }

        VkQueue getGraphicsQueue() const { return vkGraphicsQueue; }

        // Getter function for graphics queue family indices.
        VulkanQueueFamilyIndex getPresentQueueFamilyIndex() const { return presentQueueFamilyIndex; }
        
        VkQueue getPresentQueue() const { return vkPresentQueue; }
    private:
        // Initialize queue families based on the surface.
        void initQueueFamilies(Ref<VulkanSurface> surface);

        // Private member function to retrieve required Vulkan device extensions.
        std::vector<const char*> getEnabledExtensions(const GraphicsContextCreateInfo& info) const;

        // Private member function to retrieve required Vulkan device layers.
        std::vector<const char*> getEnabledLayers() const;

        // Private member function to retrieve required Vulkan device features.
        VkPhysicalDeviceFeatures getEnabledFeatures() const;

    private:
        VkPhysicalDevice vkPhysicalDevice; // Vulkan physical device
        bool selected = false; // Flag indicating whether the device is selected

        // --------------------
        // Valid when selected
        // --------------------
        VkDevice vkDevice = VK_NULL_HANDLE; // Vulkan logical device
        Ref<VulkanAllocator> allocator = nullptr; // Vulkan allocator

        VulkanQueueFamilyIndex graphicsQueueFamilyIndex = VulkanQueueFamilyIndex::None;
        VkQueue vkGraphicsQueue = VK_NULL_HANDLE; // Vulkan graphics queue

        VulkanQueueFamilyIndex presentQueueFamilyIndex = VulkanQueueFamilyIndex::None;
        VkQueue vkPresentQueue = VK_NULL_HANDLE; // Vulkan present queue
    };
}

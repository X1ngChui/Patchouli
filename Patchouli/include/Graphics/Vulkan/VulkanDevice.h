#pragma once

#include "Graphics/GraphicsDevice.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
    constexpr std::size_t VULKAN_QUEUE_FAMILY_NONE = 0xffffffff;

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

        // Structure to store queue families.
        struct VulkanQueueFamilies
        {
            uint32_t graphics = VULKAN_QUEUE_FAMILY_NONE; // Index of the graphics queue family
            uint32_t compute = VULKAN_QUEUE_FAMILY_NONE; // Index of the compute queue family
            uint32_t transfer = VULKAN_QUEUE_FAMILY_NONE; // Index of the transfer queue family
            uint32_t sparseBinding = VULKAN_QUEUE_FAMILY_NONE; // Index of the sparse binding queue family
            uint32_t present = VULKAN_QUEUE_FAMILY_NONE; // Index of the present queue family
        };

        // Getter function for queue family indices.
        const VulkanQueueFamilies& getQueueFamilies() const { return queueFamilies; }

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
        VkQueue vkGraphicsQueue = VK_NULL_HANDLE; // Vulkan graphics queue
        VkQueue vkPresentQueue = VK_NULL_HANDLE; // Vulkan present queue
        VulkanQueueFamilies queueFamilies; // Selected queue families
    };
}

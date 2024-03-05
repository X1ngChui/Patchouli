#pragma once

#include "Graphics/GraphicsDevice.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include <vulkan/vulkan.h>

#define PATCHOULI_VULKAN_QUEUE_FAMILY_NONE (0xffffffff)

namespace Patchouli
{
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

        // Private member function to retrieve required Vulkan device extensions.
        std::vector<const char*> getEnabledExtensions() const;

        // Private member function to retrieve required Vulkan device layers.
        std::vector<const char*> getEnabledLayers() const;

        // Private member function to retrieve required Vulkan device features.
        VkPhysicalDeviceFeatures getEnabledFeatures() const;

        // Selects the Vulkan device and initializes its properties.
        void onSelect(Ref<VulkanAllocator> allocator, Ref<VulkanSurface> surface);

        // Retrieves a list of Vulkan devices associated with a Vulkan instance.
        static std::vector<Ref<GraphicsDevice>> getDevices(Ref<VulkanInstance> instance);

        // Conversion operator to VkInstance.
        // It allows VulkanInstance objects to be implicitly converted to VkDevice.
        inline operator VkDevice() const { return vkDevice; }

        // Conversion operator to VkInstance.
        // It allows VulkanInstance objects to be implicitly converted to VkPhyscialDevice.
        inline operator VkPhysicalDevice() const { return vkPhysicalDevice; }

        // Structure to store queue families.
        struct QueueFamilyIndices
        {
            uint32_t graphics = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Graphics queue family index
            uint32_t compute = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Compute queue family index
            uint32_t transfer = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Transfer queue family index
            uint32_t sparseBinding = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Sparse binding queue family index
            uint32_t present = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Present queue family index
        };
        const QueueFamilyIndices& getQueueFamilies() const { return queueFamilies; }
    private:
        VkPhysicalDevice vkPhysicalDevice; // Vulkan physical device

        bool selected = false; // Flag indicating whether the device is selected

        // --------------------
        // Valid when selected
        // --------------------
        VkDevice vkDevice = VK_NULL_HANDLE; // Vulkan logical device
        Ref<VulkanAllocator> vkAllocator = nullptr; // Weak reference to Vulkan allocator

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        QueueFamilyIndices queueFamilies;
    };
}

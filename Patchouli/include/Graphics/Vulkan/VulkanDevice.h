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

    private:
        VkPhysicalDevice vkPhysicalDevice; // Vulkan physical device
        VkDevice vkDevice = VK_NULL_HANDLE; // Vulkan logical device

        bool selected = false; // Flag indicating whether the device is selected

        // --------------------
        // Valid when selected
        // --------------------
        // Structure to store Vulkan queues associated with the device.
        struct Queues
        {
            VkQueue graphics; // Graphics queue
            VkQueue compute; // Compute queue
            VkQueue transfer; // Transfer queue
            VkQueue sparseBinding; // Sparse binding queue
            VkQueue present; // Present queue
        };
        Scope<Queues> queues = nullptr; // Scoped pointer to Vulkan queues associated with the device
        WeakRef<VulkanAllocator> vkAllocator = nullptr; // Weak reference to Vulkan allocator
    };
}

#pragma once

#include "Graphics/GraphicsDevice.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include <vulkan/vulkan.h>

#define PATCHOULI_VULKAN_QUEUE_FAMILY_NONE 0xffffffff

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

        // Selects the Vulkan device and initializes its properties.
        void onSelect(Ref<VulkanAllocator> allocator);

        // Retrieves a list of Vulkan devices associated with a Vulkan instance.
        static std::vector<Ref<GraphicsDevice>> getDevices(Ref<VulkanInstance> instance);

    private:
        // Structure to store queue families.
        struct QueueFamilies
        {
            uint32_t graphics = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Graphics queue family index
            uint32_t compute = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Compute queue family index
            uint32_t transfer = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Transfer queue family index
            uint32_t sparseBinding = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Sparse binding queue family index
        };

        // Structure to store Vulkan queues associated with the device.
        struct Queues
        {
            VkQueue graphics; // Graphics queue
            VkQueue compute; // Compute queue
            VkQueue transfer; // Transfer queue
            VkQueue sparseBinding; // Sparse binding queue
        };

        // Retrieves queue family properties of the physical device.
        std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;

        // Determines supported functionalities for each queue family.
        QueueFamilies determineQueueFamilies(const std::vector<VkQueueFamilyProperties>& queueFamilyProperties) const;

        // Creates an array of VkDeviceQueueCreateInfo structs for device creation.
        std::vector<VkDeviceQueueCreateInfo> createQueueCreateInfoArray(const QueueFamilies& queueFamilies) const;

        // Creates the Vulkan device.
        void createDevice(const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfoArray, const QueueFamilies& queueFamilies);

    private:
        VkPhysicalDevice vkPhysicalDevice; // Vulkan physical device
        VkDevice vkDevice = VK_NULL_HANDLE; // Vulkan logical device

        bool selected = false; // Flag indicating whether the device is selected

        // --------------------
        // Valid when selected
        // --------------------
        Scope<Queues> queues = nullptr; // Scoped pointer to Vulkan queues associated with the device
        WeakRef<VulkanAllocator> vkAllocator = nullptr; // Weak reference to Vulkan allocator
    };
}

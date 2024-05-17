#include "Graphics/Vulkan/VulkanBase.h"
#include "Graphics/Vulkan/VulkanDevice.h"

namespace Patchouli
{
    // Constructor for VulkanDevice class
    VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice)
        : vkPhysicalDevice(physicalDevice)
    {
    }

    // Destructor for VulkanDevice class
    VulkanDevice::~VulkanDevice()
    {
        if (selected)
        {
            // Wait for the device to become idle before destroying it
            vkDeviceWaitIdle(vkDevice);
            vkDestroyDevice(vkDevice, *allocator);
        }
    }

    // Function to retrieve properties of the Vulkan device
    GraphicsDeviceProperties VulkanDevice::getProperties() const
    {
        VkPhysicalDeviceProperties vkProperties;
        vkGetPhysicalDeviceProperties(vkPhysicalDevice, &vkProperties);

        // Construct GraphicsDeviceProperties struct from Vulkan device properties
        GraphicsDeviceProperties properties = {
            .apiVersion = vkProperties.apiVersion,
            .driverVersion = vkProperties.driverVersion,
            .vendorID = vkProperties.vendorID,
            .deviceID = vkProperties.deviceID,
            .discreteGPU = (bool)(vkProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        };
        std::strncpy(properties.name, vkProperties.deviceName, PATCHOULI_DEVICE_NAME_SIZE);

        return properties;
    }

    // Function to retrieve features of the Vulkan device
    GraphicsDeviceFeatures VulkanDevice::getFeatures() const
    {
        VkPhysicalDeviceFeatures vkFeatures;
        vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &vkFeatures);

        // Construct GraphicsDeviceFeatures struct from Vulkan device features
        GraphicsDeviceFeatures features = {
            .geometryShader = (bool)vkFeatures.geometryShader
        };

        return features;
    }

    std::vector<const char*> VulkanDevice::getEnabledExtensions(const GraphicsContextCreateInfo& info) const
    {
        std::vector<const char*> extensions;
        extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        return extensions;
    }

    std::vector<const char*> VulkanDevice::getEnabledLayers() const
    {
        std::vector<const char*> layers;

#ifdef PATCHOULI_VULKAN_VALIDATION
        layers.push_back(PATCHOULI_VULKAN_VALIDATION);
#endif

        return layers;
    }

    VkPhysicalDeviceFeatures VulkanDevice::getEnabledFeatures() const
    {
        return VkPhysicalDeviceFeatures();
    }

    // Function to select the Vulkan device and initialize its properties
    void VulkanDevice::onSelect(Ref<VulkanAllocator> allocator, Ref<VulkanSurface> surface, const GraphicsContextCreateInfo& info)
    {
        selected = true; // Mark the device as selected
        this->allocator = allocator; // Store the allocator for resource management

        // Get and determine queue families to be uesd
        initQueueFamilies(surface);
        assert(queueFamilies.graphics != PATCHOULI_VULKAN_QUEUE_FAMILY_NONE);

        // Create queue create info array for device creation
        // Due to the possibility of different queues having the same index for various purposes,
        // duplicate indices are removed
        // Note: Now only graphis and present queues are used
        std::set<uint32_t> distinctIndices = { queueFamilies.graphics, queueFamilies.present };

        // Create device queue create info for each distinct queue index
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        static const float priority = 1.0f;
        for (uint32_t index : distinctIndices)
        {
            if (index == PATCHOULI_VULKAN_QUEUE_FAMILY_NONE)
                continue;

            VkDeviceQueueCreateInfo deviceQueueCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueFamilyIndex = index,
                .queueCount = 1, // Note: Now only 1 queue is created
                .pQueuePriorities = &priority
            };
            queueCreateInfos.push_back(deviceQueueCreateInfo);
        }

        // Get required extensions, layers and features
        std::vector<const char*> layers = getEnabledLayers();
        std::vector<const char*> extensions = getEnabledExtensions(info);
        VkPhysicalDeviceFeatures features = getEnabledFeatures();

        // Create Vulkan logical device
        VkDeviceCreateInfo deviceCreateinfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
            .pQueueCreateInfos = queueCreateInfos.data(),
            .enabledLayerCount = static_cast<uint32_t>(layers.size()),
            .ppEnabledLayerNames = layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
            .pEnabledFeatures = &features
        };

        // Create the Vulkan device
        VkResult status = vkCreateDevice(vkPhysicalDevice, &deviceCreateinfo, *allocator, &vkDevice);
        assert(status == VK_SUCCESS);

        // Retrieve queues for different queue families
        // Note: Now only graphis and present queues are used
        if (queueFamilies.graphics != PATCHOULI_VULKAN_QUEUE_FAMILY_NONE)
            vkGetDeviceQueue(vkDevice, queueFamilies.graphics, 0, &vkGraphicsQueue);
        if (queueFamilies.present != PATCHOULI_VULKAN_QUEUE_FAMILY_NONE)
            vkGetDeviceQueue(vkDevice, queueFamilies.present, 0, &vkPresentQueue);
    }

    // Function to retrieve a list of Vulkan devices associated with a Vulkan instance
    std::vector<Ref<GraphicsDevice>> VulkanDevice::getDevices(Ref<VulkanInstance> instance)
    {
        uint32_t nDevices = 0;
        vkEnumeratePhysicalDevices(*instance, &nDevices, nullptr);
        std::vector<VkPhysicalDevice> physicalDevices(nDevices);
        vkEnumeratePhysicalDevices(*instance, &nDevices, physicalDevices.data());

        std::vector<Ref<GraphicsDevice>> devices;

        // Transform Vulkan physical devices into GraphicsDevice objects
        std::ranges::transform(physicalDevices, std::back_inserter(devices), [](auto physicalDevice) {
            return makeRef<VulkanDevice>(physicalDevice);
            });

        return devices;
    }

    void VulkanDevice::initQueueFamilies(Ref<VulkanSurface> surface)
    {
        // Get queue family properties of the selected physical device
        uint32_t nQueueFamilies = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &nQueueFamilies, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(nQueueFamilies);
        vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &nQueueFamilies, queueFamilyProperties.data());
        assert(nQueueFamilies > 0);

        // TODO: Better strategy for queueFamilies select
        // Iterate through each queue family and check its supported functionalities
        for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
        {
            const auto& property = queueFamilyProperties[i];
            if (property.queueCount <= 0)
                continue;

            // Extract each supported functionality from the queue family's flags
            if (property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                queueFamilies.graphics = i;

            if (property.queueFlags & VK_QUEUE_COMPUTE_BIT)
                queueFamilies.compute = i;

            if (property.queueFlags & VK_QUEUE_TRANSFER_BIT)
                queueFamilies.transfer = i;

            if (property.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
                queueFamilies.sparseBinding = i;

            // Check if the queue family supports presentation
            if (*surface) // if a window surface is wanted by the application
            {
                VkBool32 presentSupported = VK_FALSE;
                vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, i, *surface, &presentSupported);
                if (presentSupported)
                    queueFamilies.present = i;
            }
        }
    }
}
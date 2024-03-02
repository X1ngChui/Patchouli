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
            vkDestroyDevice(vkDevice, *vkAllocator.lock());
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

        // Copy device name to GraphicsDeviceProperties struct
        std::strncpy(properties.deviceName, vkProperties.deviceName, PATCHOULI_MAX_DEVICE_NAME_SIZE);

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

    // Function to select the Vulkan device and initialize its properties
    void VulkanDevice::onSelect(Ref<VulkanAllocator> allocator)
    {
        selected = true; // Mark the device as selected
        vkAllocator = allocator; // Store the allocator for resource management

        // Get queue family properties of the selected physical device
        std::vector<VkQueueFamilyProperties> queueFamilyProperties = getQueueFamilyProperties();

        // Determine supported functionalities for each queue family
        QueueFamilies queueFamilies = determineQueueFamilies(queueFamilyProperties);

        // Create queue create info array for device creation
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfoArray = createQueueCreateInfoArray(queueFamilies);

        // Create Vulkan device
        createDevice(queueCreateInfoArray, queueFamilies);
    }

    std::vector<VkQueueFamilyProperties> VulkanDevice::getQueueFamilyProperties() const
    {
        uint32_t nQueueFamilies = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &nQueueFamilies, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(nQueueFamilies);
        vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &nQueueFamilies, queueFamilyProperties.data());
        return queueFamilyProperties;
    }

    Patchouli::VulkanDevice::QueueFamilies VulkanDevice::determineQueueFamilies(const std::vector<VkQueueFamilyProperties>& queueFamilyProperties) const
    {
        QueueFamilies queueFamilies;

        for (uint32_t index = 0; index < queueFamilyProperties.size(); index++)
        {
            if (queueFamilyProperties[index].queueCount <= 0)
                continue;

            VkQueueFlags flags = queueFamilyProperties[index].queueFlags;

            while (flags)
            {
                uint32_t functionality = flags & (-flags);
                flags &= ~functionality;

                switch (functionality)
                {
                case VK_QUEUE_GRAPHICS_BIT:
                    queueFamilies.graphics = index;
                    break;
                case VK_QUEUE_COMPUTE_BIT:
                    queueFamilies.compute = index;
                    break;
                case VK_QUEUE_TRANSFER_BIT:
                    queueFamilies.transfer = index;
                    break;
                case VK_QUEUE_SPARSE_BINDING_BIT:
                    queueFamilies.sparseBinding = index;
                    break;
                }
            }
        }

        return queueFamilies;
    }

    std::vector<VkDeviceQueueCreateInfo> VulkanDevice::createQueueCreateInfoArray(const QueueFamilies& queueFamilies) const
    {
        static const float priority = 1.0f;
        std::set<uint32_t> distinct = { queueFamilies.graphics, queueFamilies.compute,
                                         queueFamilies.transfer, queueFamilies.sparseBinding };
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfoArray;
        for (uint32_t queueFamily : distinct)
        {
            VkDeviceQueueCreateInfo queueCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueFamilyIndex = queueFamily,
                .queueCount = 1,
                .pQueuePriorities = &priority
            };
            queueCreateInfoArray.push_back(queueCreateInfo);
        }
        return queueCreateInfoArray;
    }

    void VulkanDevice::createDevice(const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfoArray, const QueueFamilies& queueFamilies)
    {
        std::vector<const char*> layers;
#ifdef PATCHOULI_VULKAN_VALIDATION
        layers.push_back(PATCHOULI_VULKAN_VALIDATION);
#endif

        VkPhysicalDeviceFeatures features = {};

        VkDeviceCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfoArray.size()),
            .pQueueCreateInfos = queueCreateInfoArray.data(),
            .enabledLayerCount = static_cast<uint32_t>(layers.size()),
            .ppEnabledLayerNames = layers.data(),
            .enabledExtensionCount = 0,
            .ppEnabledExtensionNames = nullptr,
            .pEnabledFeatures = &features
        };

        // Create the Vulkan device
        VkResult status = vkCreateDevice(vkPhysicalDevice, &info, *vkAllocator.lock(), &vkDevice);
        assert(status == VK_SUCCESS);

        // Retrieve queues for different queue families
        queues = makeScope<Queues>();
        vkGetDeviceQueue(vkDevice, queueFamilies.graphics, 0, &queues->graphics);
        vkGetDeviceQueue(vkDevice, queueFamilies.compute, 0, &queues->compute);
        vkGetDeviceQueue(vkDevice, queueFamilies.transfer, 0, &queues->transfer);
        vkGetDeviceQueue(vkDevice, queueFamilies.sparseBinding, 0, &queues->sparseBinding);
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
}
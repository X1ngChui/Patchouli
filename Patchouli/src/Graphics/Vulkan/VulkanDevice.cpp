#include "Graphics/Vulkan/VulkanBase.h"
#include "Graphics/Vulkan/VulkanDevice.h"

namespace Patchouli
{
    // Structure to store queue families.
    struct QueueFamilyIndices
    {
        uint32_t graphics = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Graphics queue family index
        uint32_t compute = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Compute queue family index
        uint32_t transfer = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Transfer queue family index
        uint32_t sparseBinding = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Sparse binding queue family index
        uint32_t present = PATCHOULI_VULKAN_QUEUE_FAMILY_NONE; // Present queue family index
    };

    struct IndexedQueueFamilyProperties
    {
        uint32_t index;
        VkQueueFamilyProperties properties;
    };

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

    std::vector<const char*> VulkanDevice::getEnabledExtensions() const
    {
        return std::vector<const char*>();
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
    void VulkanDevice::onSelect(Ref<VulkanAllocator> allocator, Ref<VulkanSurface> surface)
    {
        selected = true; // Mark the device as selected
        vkAllocator = allocator; // Store the allocator for resource management

        // Get queue family properties of the selected physical device
        uint32_t nQueueFamilies = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &nQueueFamilies, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(nQueueFamilies);
        vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &nQueueFamilies, queueFamilyProperties.data());

        // Make queue family properties indexed
        std::vector<IndexedQueueFamilyProperties> indexedQueueFamilyProperties;
        indexedQueueFamilyProperties.reserve(nQueueFamilies);
        for (uint32_t i = 0; i < nQueueFamilies; i++)
            indexedQueueFamilyProperties.push_back({ i, queueFamilyProperties[i] });

        // Determine supported functionalities for each queue family
        QueueFamilyIndices queueFamilyIndices;

        // Sort in descending order by queue count
        std::ranges::sort(queueFamilyProperties,
            [](const auto& a, const auto& b) {return a.queueCount > a.queueCount; });

        // Iterate through each queue family and determine its supported functionalities
        for (const auto& indexedProperty : indexedQueueFamilyProperties)
        {
            VkQueueFlags flags = indexedProperty.properties.queueFlags;

            // Extract each supported functionality from the queue family's flags
            while (flags)
            {
                VkQueueFlags usage = flags & (-flags);
                flags &= ~usage;

                switch (usage)
                {
                case VK_QUEUE_GRAPHICS_BIT:
                    if (queueFamilyIndices.graphics == PATCHOULI_VULKAN_QUEUE_FAMILY_NONE)
                        queueFamilyIndices.graphics = indexedProperty.index;
                    break;
                case VK_QUEUE_COMPUTE_BIT:
                    if (queueFamilyIndices.compute == PATCHOULI_VULKAN_QUEUE_FAMILY_NONE)
                        queueFamilyIndices.compute = indexedProperty.index;
                    break;
                case VK_QUEUE_TRANSFER_BIT:
                    if (queueFamilyIndices.transfer == PATCHOULI_VULKAN_QUEUE_FAMILY_NONE)
                        queueFamilyIndices.transfer = indexedProperty.index;
                    break;
                case VK_QUEUE_SPARSE_BINDING_BIT:
                    if (queueFamilyIndices.sparseBinding == PATCHOULI_VULKAN_QUEUE_FAMILY_NONE)
                        queueFamilyIndices.sparseBinding = indexedProperty.index;
                    break;
                }
            }

            // Check if the queue family supports presentation
            VkBool32 presentSupported = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, indexedProperty.index, *surface, &presentSupported);
            if (presentSupported && queueFamilyIndices.present == PATCHOULI_VULKAN_QUEUE_FAMILY_NONE)
                queueFamilyIndices.present = indexedProperty.index;
        }

        // Create queue create info array for device creation
        // Due to the possibility of different queues having the same index for various purposes,
        // duplicate indices are removed
        std::set<uint32_t> distinctIndex = { queueFamilyIndices.graphics, queueFamilyIndices.compute,
            queueFamilyIndices.transfer, queueFamilyIndices.sparseBinding, queueFamilyIndices.present
        };
        uint32_t nQueueCreateInfo = distinctIndex.size();

        // Create device queue create info for each distinct queue index
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfoArray;
        queueCreateInfoArray.reserve(nQueueCreateInfo);
        static const float priority = 1.0f;
        for (uint32_t index : distinctIndex)
        {
            VkDeviceQueueCreateInfo deviceQueueCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueFamilyIndex = index,
                .queueCount = 1, // queueFamilyProperties[index].queueCount,
                .pQueuePriorities = &priority
            };
            queueCreateInfoArray.push_back(deviceQueueCreateInfo);
        }

        // Create Vulkan logical device
        std::vector<const char*> layers = getEnabledLayers();
        std::vector<const char*> extensions = getEnabledExtensions();
        VkPhysicalDeviceFeatures features = getEnabledFeatures();

        VkDeviceCreateInfo deviceCreateinfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = nQueueCreateInfo,
            .pQueueCreateInfos = queueCreateInfoArray.data(),
            .enabledLayerCount = static_cast<uint32_t>(layers.size()),
            .ppEnabledLayerNames = layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
            .pEnabledFeatures = &features
        };

        // Create the Vulkan device
        VkResult status = vkCreateDevice(vkPhysicalDevice, &deviceCreateinfo, *vkAllocator.lock(), &vkDevice);
        assert(status == VK_SUCCESS);

        // Retrieve queues for different queue families
        queues = makeScope<Queues>();
        vkGetDeviceQueue(vkDevice, queueFamilyIndices.graphics, 0, &queues->graphics);
        vkGetDeviceQueue(vkDevice, queueFamilyIndices.compute, 0, &queues->compute);
        vkGetDeviceQueue(vkDevice, queueFamilyIndices.transfer, 0, &queues->transfer);
        vkGetDeviceQueue(vkDevice, queueFamilyIndices.sparseBinding, 0, &queues->sparseBinding);
        vkGetDeviceQueue(vkDevice, queueFamilyIndices.present, 0, &queues->present);
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
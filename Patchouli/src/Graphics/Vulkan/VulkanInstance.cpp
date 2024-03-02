#include "Graphics/Vulkan/VulkanBase.h"
#include "Graphics/Vulkan/VulkanContext.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include "Log/Console.h"
#include <GLFW/glfw3.h>

namespace Patchouli
{
    // Constructor for VulkanInstance.
    // It initializes Vulkan instance based on the provided GraphicsInfo and VulkanAllocator.
    VulkanInstance::VulkanInstance(Ref<VulkanAllocator> allocator)
        : vkAllocator(allocator)
    {
        // Get required Vulkan extensions and layers
        std::vector<const char*> extensions = getExtensions(VulkanContext::getWindowAPI());
        std::vector<const char*> layers = getLayers();

        // Check if all required layers are supported
        assert(checkLayers(layers));

        // Fill VkApplicationInfo structure
        VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = VulkanContext::getAppName(),
            .applicationVersion = VulkanContext::getAppVersion(),
            .pEngineName = "Patchouli",
            .engineVersion = VK_MAKE_API_VERSION(0, 0, 0, 0),
            .apiVersion = VK_API_VERSION_1_0
        };

        // Fill VkInstanceCreateInfo structure
        VkInstanceCreateInfo instanceInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = static_cast<uint32_t>(layers.size()),
            .ppEnabledLayerNames = layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data()
        };

        // Create Vulkan instance
        VkResult result = vkCreateInstance(&instanceInfo, *vkAllocator.lock(), &(this->vkInstance));
        assert(result == VK_SUCCESS);
    }

    // Destructor for VulkanInstance.
    // It cleans up Vulkan instance resources.
    VulkanInstance::~VulkanInstance()
    {
        // Destroy Vulkan instance
        vkDestroyInstance(this->vkInstance, *vkAllocator.lock());
    }

    // Private member function to retrieve required Vulkan extensions based on WindowAPI.
    std::vector<const char*> VulkanInstance::getExtensions(WindowAPI windowAPI) const
    {
        uint32_t nExtensions = 0;
        const char** rawExtensions = nullptr;

        switch (windowAPI)
        {
        case WindowAPI::GLFW:
            rawExtensions = glfwGetRequiredInstanceExtensions(&nExtensions);
            break;
        }

        std::vector<const char*> extensions(rawExtensions, rawExtensions + nExtensions);
#ifdef PATCHOULI_VULKAN_VALIDATION
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return extensions;
    }

    std::vector<const char*> VulkanInstance::getLayers() const
    {
        std::vector<const char*> layers;

#ifdef PATCHOULI_VULKAN_VALIDATION
        layers.push_back(PATCHOULI_VULKAN_VALIDATION);
#endif
        return layers;
    }

    // Private member function to check if specified Vulkan layers are supported.
    bool VulkanInstance::checkLayers(std::vector<const char*>& layers) const
    {
        // Retrieve information about supported Vulkan layers
        uint32_t nSupported = 0;
        vkEnumerateInstanceLayerProperties(&nSupported, nullptr);
        std::vector<VkLayerProperties> supported(nSupported);
        vkEnumerateInstanceLayerProperties(&nSupported, supported.data());

        // Create a set of supported layer names for efficient lookup
        std::unordered_set<std::string> supportedLayers;
        for (const auto& prop : supported)
            supportedLayers.insert(prop.layerName);

        // Check if all specified layers are supported
        return std::ranges::all_of(layers, [&](const char* layer) {
            return supportedLayers.contains(layer);
            }
        );
    }
}

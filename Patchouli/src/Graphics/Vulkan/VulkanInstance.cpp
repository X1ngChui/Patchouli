#include "Graphics/Vulkan/VulkanInstance.h"
#include "Log/Console.h"
#include <GLFW/glfw3.h>

namespace Patchouli
{
    // Constructor for VulkanInstance.
    // It initializes Vulkan instance based on the provided GraphicsInfo and VulkanAllocator.
    VulkanInstance::VulkanInstance(const GraphicsInfo& info, Ref<VulkanAllocator> allocator)
        : vkAllocator(allocator)
    {
        // Get required Vulkan extensions based on the WindowAPI
        std::vector<const char*> extensions = getExtensions(info.windowAPI);
        std::vector<const char*> layers;

        // Add validation layer and debug extension if PATCHOULI_VULKAN_VALIDATION is defined
#ifdef PATCHOULI_VULKAN_VALIDATION
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

        // Check if all required layers are supported
        Console::coreAssert(checkLayers(layers), "Not all required layers are supported.");

        // Fill VkApplicationInfo structure
        VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = info.appName,
            .applicationVersion = info.appVersion,
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
        Console::coreAssert(result == VK_SUCCESS, "Vulkan instance creation failed.");
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
        const char** extensions = nullptr;

        switch (windowAPI)
        {
        case WindowAPI::GLFW:
            extensions = glfwGetRequiredInstanceExtensions(&nExtensions);
            break;
        }

        return std::vector<const char*>(extensions, extensions + nExtensions);
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

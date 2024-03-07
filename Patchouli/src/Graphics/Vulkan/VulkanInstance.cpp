#include "Core/Application.h"
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
        std::vector<const char*> extensions = getEnabledExtensions(Application::getInstance().getAppInfo().windowInfo.windowAPI);
        std::vector<const char*> layers = getEnabledLayers();

        // Fill VkApplicationInfo structure
        VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = Application::getInstance().getAppInfo().appName.c_str(),
            .applicationVersion = Application::getInstance().getAppInfo().appVersion,
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
        VkResult result = vkCreateInstance(&instanceInfo, *vkAllocator, &(this->vkInstance));
        assert(result == VK_SUCCESS);
    }

    // Destructor for VulkanInstance.
    // It cleans up Vulkan instance resources.
    VulkanInstance::~VulkanInstance()
    {
        // Destroy Vulkan instance
        vkDestroyInstance(this->vkInstance, *vkAllocator);
    }

    // Private member function to retrieve required Vulkan extensions based on WindowAPI.
    std::vector<const char*> VulkanInstance::getEnabledExtensions(WindowAPI windowAPI) const
    {
        std::vector<const char*> extensions;

        switch (windowAPI)
        {
        case WindowAPI::GLFW:
            uint32_t nExtensions;
            const char** source = glfwGetRequiredInstanceExtensions(&nExtensions);
            extensions.resize(nExtensions);
            std::memcpy(extensions.data(), source, nExtensions * sizeof(const char*));
            break;
        }

#ifdef PATCHOULI_VULKAN_VALIDATION
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
        return extensions;
    }

    std::vector<const char*> VulkanInstance::getEnabledLayers() const
    {
        std::vector<const char*> layers;

#ifdef PATCHOULI_VULKAN_VALIDATION
        layers.push_back(PATCHOULI_VULKAN_VALIDATION);
#endif
        // Check if all required layers are supported
        assert(checkLayers(layers));

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

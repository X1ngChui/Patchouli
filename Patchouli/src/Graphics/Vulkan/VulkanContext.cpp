#include "Graphics/Vulkan/VulkanContext.h"
#include "Log/Console.h"

namespace Patchouli
{
	VulkanContext::VulkanContext(const GraphicsInfo& info)
	{
		VkApplicationInfo appInfo = {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = info.appName,
			.applicationVersion = info.appVersion,
			.pEngineName = "Patchouli",
			.engineVersion = VK_MAKE_API_VERSION(0, 0, 0, 0),
			.apiVersion = VK_API_VERSION_1_0
		};

		VkInstanceCreateInfo instanceInfo = {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = nullptr,
			.enabledExtensionCount = 0,
			.ppEnabledExtensionNames = nullptr,
		};

		VkResult result = vkCreateInstance(&instanceInfo, &allocationCallbacks, &(this->vkInstance));
		Console::coreAssert(result == VK_SUCCESS, "Vulkan instance creation failed.");

	}

	VulkanContext::~VulkanContext()
	{
		vkDestroyInstance(this->vkInstance, &allocationCallbacks);
	}
}
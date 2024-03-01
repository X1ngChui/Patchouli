#include "Graphics/Vulkan/VulkanDevice.h"

namespace Patchouli
{
	VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice)
		: vkPhysicalDevice(physicalDevice)
	{
		VkPhysicalDeviceProperties vkProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &vkProperties);

		VkPhysicalDeviceFeatures vkFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &vkFeatures);

		properties = {
			.apiVersion = vkProperties.apiVersion,
			.driverVersion = vkProperties.driverVersion,
			.vendorID = vkProperties.vendorID,
			.deviceID = vkProperties.deviceID,
			.discreteGPU = (bool)(vkProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		};
		std::strncpy(properties.deviceName, vkProperties.deviceName, PATCHOULI_MAX_DEVICE_NAME_SIZE);

		features = {
			.geometryShader = (bool)vkFeatures.geometryShader
		};
	}

	std::vector<Ref<GraphicsDevice>> VulkanDevice::getDevices(Ref<VulkanInstance> instance)
	{
		uint32_t nDevices = 0;
		vkEnumeratePhysicalDevices(*instance, &nDevices, nullptr);
		std::vector<VkPhysicalDevice> physicalDevices(nDevices);
		vkEnumeratePhysicalDevices(*instance, &nDevices, physicalDevices.data());

		std::vector<Ref<GraphicsDevice>> devices;
		devices.reserve(nDevices);

		for (uint32_t i = 0; i < nDevices; i++)
		{
			Ref<GraphicsDevice> device = makeRef<VulkanDevice>(physicalDevices[i]);
			devices.push_back(device);
		}
		
		return devices;
	}
}
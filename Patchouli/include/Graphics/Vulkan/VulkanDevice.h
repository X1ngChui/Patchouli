#pragma once

#include "Graphics/GraphicsDevice.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanDevice : public GraphicsDevice
	{
	public:
		VulkanDevice(VkPhysicalDevice physicalDevice);
		virtual ~VulkanDevice() = default;
		
		static std::vector<Ref<GraphicsDevice>> getDevices(Ref<VulkanInstance> instance);
	private:
		VkPhysicalDevice vkPhysicalDevice;
	};
}
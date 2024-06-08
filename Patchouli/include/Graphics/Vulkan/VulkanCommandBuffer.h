#pragma once
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanCommandPool.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanCommandBuffer : public RefBase<VulkanCommandBuffer>
	{
	public:
		VulkanCommandBuffer(Ref<VulkanDevice> device, Ref<VulkanCommandPool> commandPool, bool asSecondary);
		~VulkanCommandBuffer();
	
	private:
		VkCommandBuffer vkCommandBuffer;

		Ref<VulkanDevice> device = nullptr;
		Ref<VulkanCommandPool> commandPool = nullptr;
	};
}
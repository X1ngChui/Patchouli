#include "Graphics/Vulkan/VulkanCommandPool.h"

namespace Patchouli
{
	VulkanCommandPool::VulkanCommandPool(Ref<VulkanDevice> device, VkCommandPoolCreateFlags flag, Ref<VulkanAllocator> allocator)
		: device(device), allocator(allocator)
	{
		VkCommandPoolCreateInfo info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = flag,
			.queueFamilyIndex = device->getGraphicsQueueFamilyIndex()
		};

		VkResult result = vkCreateCommandPool(*device, &info, *allocator, &vkCommandPool);
		assert(result == VK_SUCCESS);
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		vkDestroyCommandPool(*device, vkCommandPool, *allocator);
	}

	Ref<VulkanPrimaryCommandBuffer> VulkanCommandPool::createPrimaryCommandBuffer()
	{
		return makeRef<VulkanPrimaryCommandBuffer>(device, this->shared_from_this());
	}

	Ref<VulkanSecondaryCommandBuffer> VulkanCommandPool::createSecondaryCommandBuffer()
	{
		return makeRef<VulkanSecondaryCommandBuffer>(device, this->shared_from_this());
	}
}
#include "Graphics/Vulkan/VulkanCommandBuffer.h"

namespace Patchouli
{
	VulkanCommandBuffer::VulkanCommandBuffer(Ref<VulkanDevice> device, Ref<VulkanCommandPool> commandPool, bool asSecondary)
		: device(device), commandPool(commandPool)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = *commandPool,
			.level = asSecondary ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};

		VkResult result = vkAllocateCommandBuffers(*device, &commandBufferAllocateInfo, &vkCommandBuffer);
		assert(result = VK_SUCCESS);
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
	}
}
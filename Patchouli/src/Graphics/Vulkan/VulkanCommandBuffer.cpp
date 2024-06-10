#include "Graphics/Vulkan/VulkanCommandPool.h"
#include "Graphics/Vulkan/VulkanCommandBuffer.h"

namespace Patchouli
{
	void VulkanCommandBuffer::begin(
		VkCommandBufferUsageFlags flags, const VkCommandBufferInheritanceInfo& inheritance)
	{
		VkCommandBufferBeginInfo beginInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = flags,
			.pInheritanceInfo = &inheritance
		};

		VkResult result = vkBeginCommandBuffer(vkCommandBuffer, &beginInfo);
		assert(result == VK_SUCCESS);
	}

	void VulkanCommandBuffer::end()
	{
		vkEndCommandBuffer(vkCommandBuffer);
	}

	void VulkanCommandBuffer::beginRenderPass(
		const VkRenderPassBeginInfo& beginInfo, const VkSubpassContents& subpassContents)
	{
		vkCmdBeginRenderPass(vkCommandBuffer, &beginInfo, subpassContents);
	}

	void VulkanCommandBuffer::endRenderPass()
	{
		vkCmdEndRenderPass(vkCommandBuffer);
	}

	void VulkanCommandBuffer::bindGraphicsPipeline(VkPipeline pipeline)
	{
		vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	}

	void VulkanCommandBuffer::draw(std::size_t nVertex)
	{
		vkCmdDraw(vkCommandBuffer, nVertex, 1, 0, 0);
	}

	VulkanPrimaryCommandBuffer::VulkanPrimaryCommandBuffer(Ref<VulkanDevice> device, Ref<VulkanCommandPool> commandPool)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = *commandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		};

		VkResult result = vkAllocateCommandBuffers(*device, &commandBufferAllocateInfo, &vkCommandBuffer);
		assert(result == VK_SUCCESS);
	}

	VulkanSecondaryCommandBuffer::VulkanSecondaryCommandBuffer(Ref<VulkanDevice> device, Ref<VulkanCommandPool> commandPool)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = *commandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY,
			.commandBufferCount = 1
		};

		VkResult result = vkAllocateCommandBuffers(*device, &commandBufferAllocateInfo, &vkCommandBuffer);
		assert(result = VK_SUCCESS);
	}
}
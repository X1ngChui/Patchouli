#pragma once
#include "Graphics/Vulkan/VulkanDevice.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanCommandPool;

	class VulkanCommandBuffer : public RefBase<VulkanCommandBuffer>
	{
	public:
		virtual ~VulkanCommandBuffer() = default;

		void begin(VkCommandBufferUsageFlags flags = 0, const VkCommandBufferInheritanceInfo& inheritance = {});
		void end();

		void beginRenderPass(const VkRenderPassBeginInfo& beginInfo, const VkSubpassContents& subpassContents);
		void endRenderPass();

		void bindGraphicsPipeline(VkPipeline pipeline);
		void draw(std::size_t nVertex);
	
	protected:
		VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
	};

	class VulkanPrimaryCommandBuffer : public VulkanCommandBuffer
	{
	public:
		VulkanPrimaryCommandBuffer(Ref<VulkanDevice> device, Ref<VulkanCommandPool> commandPool);
		virtual ~VulkanPrimaryCommandBuffer() = default;
	};

	class VulkanSecondaryCommandBuffer : public VulkanCommandBuffer
	{
	public:
		VulkanSecondaryCommandBuffer(Ref<VulkanDevice> device, Ref<VulkanCommandPool> commandPool);
		virtual ~VulkanSecondaryCommandBuffer() = default;
	};
}
#pragma once
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Graphics/Vulkan/VulkanCommandBuffer.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanCommandPool : public RefBase<VulkanCommandPool>
	{
	public:
		VulkanCommandPool(Ref<VulkanDevice> device, VkCommandPoolCreateFlags flag, Ref<VulkanAllocator> allocator);
		~VulkanCommandPool();

		operator VkCommandPool() const { return vkCommandPool; }

		Ref<VulkanPrimaryCommandBuffer> createPrimaryCommandBuffer();
		Ref<VulkanSecondaryCommandBuffer> createSecondaryCommandBuffer();

	private:
		VkCommandPool vkCommandPool = VK_NULL_HANDLE;

		Ref<VulkanDevice> device = nullptr;
		Ref<VulkanAllocator> allocator = nullptr;
	};
}
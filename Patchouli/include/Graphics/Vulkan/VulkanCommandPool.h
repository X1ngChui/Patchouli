#pragma once
#include "Core/PObject.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanCommandPool : public PObject
	{
	public:
		VulkanCommandPool(Ref<VulkanDevice> device, VkCommandPoolCreateFlags flag, Ref<VulkanAllocator> allocator);
		~VulkanCommandPool();

	private:
		VkCommandPool vkCommandPool = VK_NULL_HANDLE;

		Ref<VulkanDevice> device = nullptr;
		Ref<VulkanAllocator> allocator = nullptr;
	};
}
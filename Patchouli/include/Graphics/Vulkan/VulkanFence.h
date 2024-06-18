#pragma once
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanFence : public RefBase<VulkanFence>
	{
	public:
		VulkanFence(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator, bool signaled = true);
		~VulkanFence();
		
		void reset();
		void wait();

		operator VkFence() const { return vkFence; }

	private:
		VkFence vkFence = VK_NULL_HANDLE;

		Ref<VulkanDevice> device;
		Ref<VulkanAllocator> allocator;
	};
}
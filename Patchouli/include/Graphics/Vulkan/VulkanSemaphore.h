#pragma once
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanSemaphore : public RefBase<VulkanSemaphore>
	{
	public:
		VulkanSemaphore(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator);
		~VulkanSemaphore();

		operator VkSemaphore() const { return vkSemaphore; }

	private:
		VkSemaphore vkSemaphore = VK_NULL_HANDLE;

		Ref<VulkanDevice> device;
		Ref<VulkanAllocator> allocator;
	};
}
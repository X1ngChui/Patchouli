#include "Graphics/Vulkan/VulkanSemaphore.h"

namespace Patchouli
{
	VulkanSemaphore::VulkanSemaphore(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator)
		: device(device), allocator(allocator)
	{
		constexpr VkSemaphoreCreateInfo info = {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
		};

		VkResult result = vkCreateSemaphore(*device, &info, *allocator, &vkSemaphore);
		assert(result == VK_SUCCESS);
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
		vkDestroySemaphore(*device, vkSemaphore, *allocator);
	}	
}
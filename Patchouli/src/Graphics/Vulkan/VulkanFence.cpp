#include "Graphics/Vulkan/VulkanFence.h"

namespace Patchouli
{
	VulkanFence::VulkanFence(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator, bool signaled)
		: device(device), allocator(allocator)
	{
		VkFenceCreateInfo info = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0u,
		};

		VkResult result = vkCreateFence(*device, &info, *allocator, &vkFence);
		assert(result == VK_SUCCESS);
	}

	VulkanFence::~VulkanFence()
	{
		VkResult result = vkWaitForFences(*device, 1, &vkFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
		assert(result == VK_SUCCESS);
		vkDestroyFence(*device, vkFence, *allocator);
	}

	void VulkanFence::reset()
	{
		VkResult result = vkResetFences(*device, 1, &vkFence);
		assert(result == VK_SUCCESS);
	}

	void VulkanFence::wait()
	{
		VkResult result = vkWaitForFences(*device, 1, &vkFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
		assert(result == VK_SUCCESS);
	}
}
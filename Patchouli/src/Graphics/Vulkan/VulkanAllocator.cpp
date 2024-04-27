#include "Graphics/Vulkan/VulkanAllocator.h"
#include <mimalloc.h>

namespace Patchouli
{
	VulkanAllocator::VulkanAllocator()
	{
		heap = mi_heap_new();

		vkCallbacks = {
			.pUserData = this,
			.pfnAllocation = &VulkanAllocator::allocation,
			.pfnReallocation = &VulkanAllocator::reallocation,
			.pfnFree = &VulkanAllocator::free,
			.pfnInternalAllocation = nullptr,
			.pfnInternalFree = nullptr
		};
	}

	VulkanAllocator::~VulkanAllocator()
	{
		mi_heap_destroy(heap);
	}

	void* VulkanAllocator::allocationImpl(std::size_t size, std::size_t alignment, VkSystemAllocationScope allocationScope)
	{
		(void)allocationScope;
		return mi_heap_malloc_aligned(heap, size, alignment);
	}

	void* VulkanAllocator::reallocationImpl(void* pOriginal, std::size_t size, std::size_t alignment, VkSystemAllocationScope allocationScope)
	{
		(void)allocationScope;
		return mi_heap_realloc_aligned(heap, pOriginal, size, alignment);
	}

	void VulkanAllocator::freeImpl(void* pMemory)
	{
		mi_free(pMemory);
	}
}
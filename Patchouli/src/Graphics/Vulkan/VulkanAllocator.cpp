#include "Graphics/Vulkan/VulkanAllocator.h"
#include <mimalloc.h>

namespace Patchouli
{
	void* VulkanAllocator::allocationImpl(std::size_t size, std::size_t alignment, VkSystemAllocationScope allocationScope)
	{
		(void)allocationScope;
		return mi_malloc_aligned(size, alignment);
	}

	void* VulkanAllocator::reallocationImpl(void* pOriginal, std::size_t size, std::size_t alignment, VkSystemAllocationScope allocationScope)
	{
		(void)allocationScope;
		return mi_realloc_aligned(pOriginal, size, alignment);
	}

	void VulkanAllocator::freeImpl(void* pMemory)
	{
		mi_free(pMemory);
	}
}
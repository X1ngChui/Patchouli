#pragma once
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanAllocator
	{
	public:
		inline operator VkAllocationCallbacks() const
		{
			VkAllocationCallbacks result = {
				.pUserData = (void*)this,
				.pfnAllocation = &allocation,
				.pfnReallocation = &reallocation,
				.pfnFree = &free,
				.pfnInternalAllocation = nullptr,
				.pfnInternalFree = nullptr
			};
			return result;
		}
	private:
		void* allocationImpl(std::size_t size, std::size_t alignment, VkSystemAllocationScope allocationScope);
		void* reallocationImpl(void* pOriginal, std::size_t size, std::size_t alignment,
			VkSystemAllocationScope allocationScope);
		void freeImpl(void* pMemory);

		static void* VKAPI_CALL allocation(void* pUserData, std::size_t size, std::size_t alignment,
			VkSystemAllocationScope allocationScope)
		{
			return static_cast<VulkanAllocator*>(pUserData)->allocationImpl(size, alignment, allocationScope);
		}

		static void* VKAPI_CALL reallocation(void* pUserData, void* pOriginal, std::size_t size, std::size_t alignment,
			VkSystemAllocationScope allocationScope)
		{
			return static_cast<VulkanAllocator*>(pUserData)->reallocationImpl(pOriginal, size, alignment, allocationScope);
		}

		static void VKAPI_CALL free(void* pUserData, void* pMemory)
		{
			return static_cast<VulkanAllocator*>(pUserData)->freeImpl(pMemory);
		}
	};
}
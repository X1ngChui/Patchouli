#pragma once
#include "Core/PObject.h"
#include <mimalloc.h>
#include <vulkan/vulkan.h>

namespace Patchouli
{
    // VulkanAllocator class is responsible for managing Vulkan memory allocation.
    class VulkanAllocator : public PObject
    {
    public:
        // Constructor initializes Vulkan allocation callbacks.
        VulkanAllocator();

        ~VulkanAllocator() = default;

        // Conversion operator to const VkAllocationCallbacks*.
        // This allows VulkanAllocator objects to be used directly as Vulkan allocation callbacks.
        inline operator const VkAllocationCallbacks*() const
        {
            return &vkCallbacks;
        }

    private:
        // Internal implementation functions for allocation, reallocation, and freeing memory.
        void* allocationImpl(std::size_t size, std::size_t alignment, VkSystemAllocationScope allocationScope);
        void* reallocationImpl(void* pOriginal, std::size_t size, std::size_t alignment,
            VkSystemAllocationScope allocationScope);
        void freeImpl(void* pMemory);

        // Static callback functions required by Vulkan for allocation, reallocation, and freeing memory.
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
    private:
        // Vulkan allocation callbacks structure.
        VkAllocationCallbacks vkCallbacks;
    };
}

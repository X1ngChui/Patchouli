#pragma once
#include "Graphics/GraphicsContext.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(const GraphicsInfo& info);
		virtual ~VulkanContext();
	private:
		VkInstance vkInstance;
		VkAllocationCallbacks allocationCallbacks = VulkanAllocator();
	};
}
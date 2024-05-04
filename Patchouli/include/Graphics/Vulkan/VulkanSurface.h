#pragma once

#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Window/Window.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanSurface : public PObject
	{
	public:
		VulkanSurface(Ref<VulkanInstance> instance, Ref<VulkanAllocator> allocator,
			Ref<Window> window);

		virtual ~VulkanSurface();

		inline operator VkSurfaceKHR() const { assert(vkSurface != VK_NULL_HANDLE); return vkSurface; }
		inline explicit operator bool() { return vkSurface != VK_NULL_HANDLE; }
	private:
		VkSurfaceKHR vkSurface = VK_NULL_HANDLE;
		Ref<VulkanInstance> instance;
		Ref<VulkanAllocator> allocator;
		Ref<Window> window;
	};
}
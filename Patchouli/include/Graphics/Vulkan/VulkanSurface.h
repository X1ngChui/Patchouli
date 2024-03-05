#pragma once

#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Window/Window.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanSurface : public GraphicsObject
	{
	public:
		VulkanSurface(Ref<VulkanInstance> instance, Ref<VulkanAllocator> allocator,
			Ref<Window> window);

		virtual ~VulkanSurface();

		inline operator VkSurfaceKHR() const { return vkSurface; }
	private:
		VkSurfaceKHR vkSurface;
		Ref<VulkanInstance> vkInstance;
		Ref<VulkanAllocator> vkAllocator;
	};
}
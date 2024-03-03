#pragma once

#include "Graphics/GraphicsSurface.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Window/Window.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanSurface : public GraphicsSurface
	{
	public:
		VulkanSurface(Ref<VulkanInstance> instance, Ref<VulkanAllocator> allocator,
			Ref<Window> window);

		virtual ~VulkanSurface();

		inline operator VkSurfaceKHR() const { return vkSurface; }
	private:
		VkSurfaceKHR vkSurface;
		WeakRef<VulkanInstance> vkInstance;
		WeakRef<VulkanAllocator> vkAllocator;
	};
}
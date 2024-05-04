#include "Graphics/Vulkan/VulkanSurface.h"
#include <GLFW/glfw3.h>

namespace Patchouli
{
	Patchouli::VulkanSurface::VulkanSurface(Ref<VulkanInstance> instance, Ref<VulkanAllocator> allocator, Ref<Window> window)
		: instance(instance), allocator(allocator), window(window)
	{
		switch (window->getAPI())
		{
		case WindowAPI::None:
			vkSurface = VK_NULL_HANDLE;
			break;
		case WindowAPI::GLFW:
			VkResult status = glfwCreateWindowSurface(*instance, *(GLFWwindow**)window->getNative(), *allocator, &vkSurface);
			assert(status == VK_SUCCESS);
			break;
		}
	}

	Patchouli::VulkanSurface::~VulkanSurface()
	{
		switch (window->getAPI())
		{
		case WindowAPI::None:
			break;
		case WindowAPI::GLFW:
			vkDestroySurfaceKHR(*instance, vkSurface, *allocator);
			break;
		}
	}
}
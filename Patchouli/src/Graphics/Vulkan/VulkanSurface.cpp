#include "Graphics/Vulkan/VulkanSurface.h"
#include <GLFW/glfw3.h>

namespace Patchouli
{
	Patchouli::VulkanSurface::VulkanSurface(Ref<VulkanInstance> instance, Ref<VulkanAllocator> allocator, Ref<Window> window)
		: vkInstance(instance), vkAllocator(allocator), window(window)
	{
		switch (window->getAPI())
		{
		case WindowAPI::None:
			vkSurface = VK_NULL_HANDLE;
			break;
		case WindowAPI::GLFW:
			VkResult status = glfwCreateWindowSurface(*vkInstance, *(GLFWwindow**)window->getNative(), *vkAllocator, &vkSurface);
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
			vkDestroySurfaceKHR(*vkInstance, vkSurface, *vkAllocator);
			break;
		}
	}
}
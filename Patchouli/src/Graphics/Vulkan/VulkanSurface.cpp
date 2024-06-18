#include "Graphics/Vulkan/VulkanSurface.h"
#include <Window/glfw/GLFWWindow.h>
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
			VkResult status = glfwCreateWindowSurface(*instance, *(GLFWWindow::NativeData)window->getNativeData(), *allocator, &vkSurface);
			assert(status == VK_SUCCESS);
			break;
		}
	}

	Patchouli::VulkanSurface::~VulkanSurface()
	{
		if (vkSurface != VK_NULL_HANDLE)
			vkDestroySurfaceKHR(*instance, vkSurface, *allocator);
	}
}
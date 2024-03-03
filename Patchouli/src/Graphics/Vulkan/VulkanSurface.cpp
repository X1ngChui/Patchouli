#include "Graphics/Vulkan/VulkanSurface.h"
#include "Core/Application.h"
#include <GLFW/glfw3.h>

namespace Patchouli
{
	Patchouli::VulkanSurface::VulkanSurface(Ref<VulkanInstance> instance, Ref<VulkanAllocator> allocator,
		Ref<Window> window)
		: vkInstance(instance), vkAllocator(allocator)
	{
		switch (Application::getInstance().getAppInfo().windowInfo.windowAPI)
		{
		case WindowAPI::GLFW:
			VkResult status = glfwCreateWindowSurface(*vkInstance.lock(), *(GLFWwindow**)window->getNative(), *vkAllocator.lock(), &vkSurface);
			assert(status == VK_SUCCESS);
			break;
		}
	}

	Patchouli::VulkanSurface::~VulkanSurface()
	{
		switch (Application::getInstance().getAppInfo().windowInfo.windowAPI)
		{
		case WindowAPI::GLFW:
			vkDestroySurfaceKHR(*vkInstance.lock(), vkSurface, *vkAllocator.lock());
			break;
		}
	}
}
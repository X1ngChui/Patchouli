#include "Window/Window.h"
#include "Window/glfw/GLFWWindow.h"

namespace Patchouli
{
	Ref<Window> Window::create(const WindowCreateInfo& info)
	{
		switch (info.windowAPI)
		{
		case WindowAPI::None:
			return nullptr;
		case WindowAPI::GLFW:
			return makeRef<GLFWWindow>(info);
		}
		return nullptr;
	}
}
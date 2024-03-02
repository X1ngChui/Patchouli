#include "Window/Window.h"
#include "Window/glfw/GLFWWindow.h"

namespace Patchouli
{
	Ref<Window> Window::create(const WindowInfo& info)
	{
		switch (info.windowAPI)
		{
		case WindowAPI::GLFW:
			return makeRef<GLFWWindow>(info);
		}
		return nullptr;
	}
}
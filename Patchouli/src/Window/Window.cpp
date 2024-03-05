#include "Window/Window.h"
#include "Window/glfw/GLFWWindow.h"
#include "Window/None/NoneWindow.h"

namespace Patchouli
{
	Ref<Window> Window::create(const WindowInfo& info)
	{
		switch (info.windowAPI)
		{
		case WindowAPI::None:
			return makeRef<NoneWindow>(info);
		case WindowAPI::GLFW:
			return makeRef<GLFWWindow>(info);
		}
		return nullptr;
	}
}
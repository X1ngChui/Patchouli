#include "Graphics/Window.h"
#include "Graphics/glfw/GLFWWindow.h"

namespace Patchouli
{
	Ref<Window> Window::create(const WindowInfo& info)
	{
		return makeRef<GLFWWindow>(info);
	}
}
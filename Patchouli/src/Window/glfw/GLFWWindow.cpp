#include "Log/Console.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Window/glfw/GLFWWindow.h"
#include "Window/glfw/GLFWProxy.h"

namespace Patchouli
{
	GLFWWindow::GLFWWindow(const WindowCreateInfo& info)
		: attribute(
			{
				.size = info.windowSize,
				.eventCallback = info.windowEventCallback
			}
		)
	{
		std::strncpy(attribute.title, info.windowTitle, PATCHOULI_WINDOW_TITLE_SIZE);
		
		window = GLFWProxy::getInstance().createWindow(info);
		GLFWProxy::getInstance().setWindowUserData(window, &attribute);

		// Set glfw callbacks
		constexpr GLFWWindowCallbacks callbacks = {
			.closeCallback = [](GLFWwindow* window)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->eventCallback(makeRef<WindowCloseEvent>());
			},

			.resizeCallback = [](GLFWwindow* window, int width, int height)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->size = { width, height };
				attrib->eventCallback(makeRef<WindowResizeEvent>(width, height));
			},

			.keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);

				switch (action)
				{
				case GLFW_PRESS:
					attrib->eventCallback(makeRef<KeyPressedEvent>(key, false));
					break;
				case GLFW_REPEAT:
					attrib->eventCallback(makeRef<KeyPressedEvent>(key, true));
					break;
				case GLFW_RELEASE:
					attrib->eventCallback(makeRef<KeyReleasedEvent>(key));
					break;
				}
			},

			.charCallback = [](GLFWwindow* window, unsigned int character)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->eventCallback(makeRef<KeyTypedEvent>(character));
			},

			.mouseButtonCallback = [](GLFWwindow* window, int button, int action, int mods)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);

				switch (action)
				{
				case GLFW_PRESS:
					attrib->eventCallback(makeRef<MouseButtonPressedEvent>(button));
					break;
				case GLFW_RELEASE:
					attrib->eventCallback(makeRef<MouseButtonReleasedEvent>(button));
					break;
				}
			},

			.scrollCallback = [](GLFWwindow* window, double xoffset, double yoffset)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->eventCallback(makeRef<MouseScrolledEvent>((float)xoffset, (float)yoffset));
			},

			.cursorPosCallback = [](GLFWwindow* window, double xpos, double ypos)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->eventCallback(makeRef<MouseMovedEvent>((float)xpos, (float)ypos));
			}
		};

		GLFWProxy::getInstance().setCallbacks(window, callbacks);
	}

	GLFWWindow::~GLFWWindow()
	{
		GLFWProxy::getInstance().destroyWindow(window);
	}

	void GLFWWindow::show()
	{
		GLFWProxy::getInstance().showWindow(window);
	}

	void GLFWWindow::hide()
	{
		GLFWProxy::getInstance().hideWindow(window);
	}

	void GLFWWindow::onUpdate()
	{
		GLFWProxy::getInstance().pollEvents();
	}
}
#include "Log/Console.h"
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
		std::strncpy(attribute.title, info.windowTitle, WINDOW_TITLE_SIZE);
		
		window = GLFWProxy::getInstance().createWindow(info);
		GLFWProxy::getInstance().setWindowUserData(window, &attribute);

		// Set glfw callbacks
		constexpr GLFWWindowCallbacks callbacks = {
			.closeCallback = [](GLFWwindow* window)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->eventCallback(WindowCloseEvent());
			},

			.resizeCallback = [](GLFWwindow* window, int width, int height)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->size = { width, height };
				attrib->eventCallback(WindowResizeEvent(width, height));
			},

			.keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);

				switch (action)
				{
				case GLFW_PRESS:
					attrib->eventCallback(KeyPressedEvent(key, false));
					break;
				case GLFW_REPEAT:
					attrib->eventCallback(KeyPressedEvent(key, true));
					break;
				case GLFW_RELEASE:
					attrib->eventCallback(KeyReleasedEvent(key));
					break;
				}
			},

			.charCallback = [](GLFWwindow* window, unsigned int character)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->eventCallback(KeyTypedEvent(character));
			},

			.mouseButtonCallback = [](GLFWwindow* window, int button, int action, int mods)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);

				switch (action)
				{
				case GLFW_PRESS:
					attrib->eventCallback(MouseButtonPressedEvent(button));
					break;
				case GLFW_RELEASE:
					attrib->eventCallback(MouseButtonReleasedEvent(button));
					break;
				}
			},

			.scrollCallback = [](GLFWwindow* window, double xoffset, double yoffset)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->eventCallback(MouseScrolledEvent((float)xoffset, (float)yoffset));
			},

			.cursorPosCallback = [](GLFWwindow* window, double xpos, double ypos)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->eventCallback(MouseMovedEvent((float)xpos, (float)ypos));
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
#include "Window/glfw/GLFWWindow.h"
#include "Log/Console.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"

namespace Patchouli
{
	GLFWWindow::GLFWWindow(const WindowInfo& info)
	{
		if (!glfwInitialized)
		{
			auto status = glfwInit();
			assert(status == GLFW_TRUE);
			glfwInitialized = true;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		this->window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.windowTitle.c_str(), nullptr, nullptr);
		assert(this->window);

		glfwSetWindowUserPointer(this->window, &(this->attribute));

		// Set glfw callbacks
		glfwSetErrorCallback([](int error_code, const char* description)
			{
				Console::coreError("glfwError({0}): {1}", error_code, description);
			});

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
			{
				WindowResizeEvent event(width, height);

				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->width = width;
				attrib->height = height;
				attrib->eventCallback(event);
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
			{
				WindowCloseEvent event;

				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->eventCallback(event);
			});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);

				switch (action) 
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent keyPressedEvent(key, false);
					attrib->eventCallback(keyPressedEvent);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent keyRepeatedEvent(key, true);
					attrib->eventCallback(keyRepeatedEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent keyReleasedEvent(key);
					attrib->eventCallback(keyReleasedEvent);
					break;
				}
				}
			});

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int character)
			{
				KeyTypedEvent event(character);

				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->eventCallback(event);
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent mousePressedEvent(button);
					attrib->eventCallback(mousePressedEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent mouseReleasedEvent(button);
					attrib->eventCallback(mouseReleasedEvent);
					break;
				}
				}
			});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				MouseScrolledEvent event((float)xoffset, (float)yoffset);

				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->eventCallback(event);
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
			{
				MouseMovedEvent event((float)xpos, (float)ypos);

				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->eventCallback(event);
			});
	}

	GLFWWindow::~GLFWWindow()
	{
		glfwDestroyWindow(this->window);
	}

	void GLFWWindow::show()
	{
		glfwShowWindow(this->window);
	}

	void GLFWWindow::hide()
	{
		glfwHideWindow(this->window);
	}

	void GLFWWindow::onUpdate()
	{
		glfwPollEvents();
	}
}
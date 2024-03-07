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
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->width = width;
				attrib->height = height;
				attrib->eventCallback(new WindowResizeEvent(width, height));
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->eventCallback(new WindowCloseEvent);
			});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
					attrib->eventCallback(new KeyPressedEvent(key, false));
					break;
				case GLFW_REPEAT:
					attrib->eventCallback(new KeyPressedEvent(key, true));
					break;
				case GLFW_RELEASE:
					attrib->eventCallback(new KeyReleasedEvent(key));
					break;
				}
			});

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int character)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->eventCallback(new KeyTypedEvent(character));
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
					attrib->eventCallback(new MouseButtonPressedEvent(button));
					break;
				case GLFW_RELEASE:
					attrib->eventCallback(new MouseButtonReleasedEvent(button));
					break;
				}
			});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->eventCallback(new MouseScrolledEvent((float)xoffset, (float)yoffset));
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->eventCallback(new MouseMovedEvent((float)xpos, (float)ypos));
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
#include "Log/Console.h"
#include "Window/glfw/GLFWWindow.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include <mimalloc.h>

namespace Patchouli
{
	class GLFWProxy
	{
	public:
		GLFWProxy()
		{
			GLFWallocator allocator = {
				.allocate = [](std::size_t size, void*) { return mi_malloc(size); },
				.reallocate = [](void* block, std::size_t size, void*) { return mi_realloc(block, size); },
				.deallocate = [](void* block, void*) { mi_free(block); },
				.user = nullptr
			};

			glfwInitAllocator(&allocator);
			auto status = glfwInit();
			assert(status == GLFW_TRUE);
		}

		~GLFWProxy()
		{
			glfwTerminate();
		}

		GLFWProxy(const GLFWProxy&) = delete;
		GLFWProxy& operator=(const GLFWProxy&) = delete;
	};

	GLFWWindow::GLFWWindow(const WindowCreateInfo& info)
	{
		attribute = {
			.width = info.windowWidth,
			.height = info.windowHeight,
			.eventCallback = info.windowEventCallback
		};

		std::strncpy(attribute.title, info.windowTitle, PATCHOULI_WINDOW_TITLE_SIZE);

		static GLFWProxy glfwProxy;

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		window = glfwCreateWindow(attribute.width, attribute.height, attribute.title, nullptr, nullptr);
		assert(window != nullptr);

		glfwSetWindowUserPointer(window, &attribute);

		// Set glfw callbacks
		glfwSetErrorCallback([](int error_code, const char* description)
			{
				Console::coreError("glfwError({}): {}", error_code, description);
			});

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->width = width;
				attrib->height = height;
				attrib->eventCallback(makeRef<WindowResizeEvent>(width, height));
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->eventCallback(makeRef<WindowCloseEvent>());
			});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);

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
			});

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int character)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->eventCallback(makeRef<KeyTypedEvent>(character));
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
					attrib->eventCallback(makeRef<MouseButtonPressedEvent>(button));
					break;
				case GLFW_RELEASE:
					attrib->eventCallback(makeRef<MouseButtonReleasedEvent>(button));
					break;
				}
			});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->eventCallback(makeRef<MouseScrolledEvent>((float)xoffset, (float)yoffset));
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowAttribute* attrib = (WindowAttribute*)glfwGetWindowUserPointer(window);
				attrib->eventCallback(makeRef<MouseMovedEvent>((float)xpos, (float)ypos));
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
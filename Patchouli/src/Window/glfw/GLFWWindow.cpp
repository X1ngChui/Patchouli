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
		GLFWProxy(const GLFWProxy&) = delete;
		GLFWProxy& operator=(const GLFWProxy&) = delete;

		static GLFWProxy& getInstance()
		{
			static GLFWProxy proxy;
			return proxy;
		}

		GLFWwindow* createWindow(const WindowCreateInfo& info)
		{
			// Set window hints
			glfwWindowHint(GLFW_RESIZABLE, test(info.flags, WindowResizable));
			glfwWindowHint(GLFW_DECORATED, test(info.flags, WindowDecorated));

			GLFWwindow* window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.windowTitle, nullptr, nullptr);
			assert(window != nullptr);
			return window;
		}

		void destroyWindow(GLFWwindow* window)
		{
			glfwDestroyWindow(window);
		}

		void showWindow(GLFWwindow* window)
		{
			glfwShowWindow(window);
		}

		void hideWindow(GLFWwindow* window)
		{
			glfwHideWindow(window);
		}

		void pollEvents()
		{
			glfwPollEvents();
		}

		void setWindowUserData(GLFWwindow* window, void* data)
		{
			glfwSetWindowUserPointer(window, data);
		}

		void* getWindowUserData(GLFWwindow* window)
		{
			return glfwGetWindowUserPointer(window);
		}

		void setWindowResizeCallback(GLFWwindow* window, GLFWwindowsizefun func)
		{
			glfwSetWindowSizeCallback(window, func);
		}

		void setWindowCloseCallback(GLFWwindow* window, GLFWwindowclosefun func)
		{
			glfwSetWindowCloseCallback(window, func);
		}

		void setKeyCallback(GLFWwindow* window, GLFWkeyfun func)
		{
			glfwSetKeyCallback(window, func);
		}

		void setCharCallback(GLFWwindow* window, GLFWcharfun func)
		{
			glfwSetCharCallback(window, func);
		}

		void setMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun func)
		{
			glfwSetMouseButtonCallback(window, func);
		}

		void setScrollCallback(GLFWwindow* window, GLFWscrollfun func)
		{
			glfwSetScrollCallback(window, func);
		}

		void setCursorPosCallback(GLFWwindow* winodw, GLFWcursorposfun func)
		{
			glfwSetCursorPosCallback(winodw, func);
		}
	private:
		GLFWProxy()
		{
			// Set allocator for glfw
			constexpr GLFWallocator allocator = {
				.allocate = [](std::size_t size, void*) { return mi_malloc(size); },
				.reallocate = [](void* block, std::size_t size, void*) { return mi_realloc(block, size); },
				.deallocate = [](void* block, void*) { mi_free(block); },
				.user = nullptr
			};
			glfwInitAllocator(&allocator);

			// Initialize glfw
			auto status = glfwInit();
			assert(status == GLFW_TRUE);

			// Set glfw error output
			glfwSetErrorCallback([](int error_code, const char* description)
				{
					Console::coreError("glfwError({}): {}", error_code, description);
				}
			);

			// Set default window options
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
			glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
			glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
			glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
		}

		~GLFWProxy()
		{
			glfwTerminate();
		}

		inline int test(WindowOptionFlags flags, WindowOptionFlagBits bit)
		{
			return (flags & bit) ? GL_TRUE : GL_FALSE;
		}
	};

	GLFWWindow::GLFWWindow(const WindowCreateInfo& info)
		: attribute({
			.width = info.windowWidth,
			.height = info.windowHeight,
			.eventCallback = info.windowEventCallback
			})
	{
		std::strncpy(attribute.title, info.windowTitle, PATCHOULI_WINDOW_TITLE_SIZE);
		
		auto& glfwProxy = GLFWProxy::getInstance();
		window = glfwProxy.createWindow(info);

		glfwProxy.setWindowUserData(window, &attribute);

		// Set glfw callbacks
		glfwProxy.setWindowResizeCallback(window, [](GLFWwindow* window, int width, int height)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->width = width;
				attrib->height = height;
				attrib->eventCallback(makeRef<WindowResizeEvent>(width, height));
			}
		);

		glfwProxy.setWindowCloseCallback(window, [](GLFWwindow* window)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->eventCallback(makeRef<WindowCloseEvent>());
			}
		);

		glfwProxy.setKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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
			});

		glfwProxy.setCharCallback(window, [](GLFWwindow* window, unsigned int character)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->eventCallback(makeRef<KeyTypedEvent>(character));
			});

		glfwProxy.setMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
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
			});

		glfwProxy.setScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->eventCallback(makeRef<MouseScrolledEvent>((float)xoffset, (float)yoffset));
			});

		glfwProxy.setCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
			{
				auto attrib = (WindowAttribute*)GLFWProxy::getInstance().getWindowUserData(window);
				attrib->eventCallback(makeRef<MouseMovedEvent>((float)xpos, (float)ypos));
			});
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
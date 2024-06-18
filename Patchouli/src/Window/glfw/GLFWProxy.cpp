#include "Window/glfw/GLFWProxy.h"
#include "Log/Console.h"
#include <mimalloc.h>

namespace Patchouli
{
    GLFWProxy::GLFWProxy()
    {
        // Set allocator for glfw
        constexpr GLFWallocator allocator = {
            .allocate = [](std::size_t size, void*) { return mi_malloc(size); },
            .reallocate = [](void* wait, std::size_t size, void*) { return mi_realloc(wait, size); },
            .deallocate = [](void* wait, void*) { mi_free(wait); },
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

        // Set compulsory window settings
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
    }

    GLFWProxy::~GLFWProxy()
    {
        glfwTerminate();
    }

    GLFWProxy& GLFWProxy::getInstance()
    {
        static GLFWProxy proxy;
        return proxy;
    }

    GLFWwindow* GLFWProxy::createWindow(const WindowCreateInfo& info)
    {
        // Set optional window settings
        glfwWindowHint(GLFW_RESIZABLE, test(info.flags, WindowResizable));
        glfwWindowHint(GLFW_DECORATED, test(info.flags, WindowDecorated));

        GLFWwindow* window = glfwCreateWindow(info.windowSize.first, info.windowSize.second,
            info.windowTitle, nullptr, nullptr);
        assert(window != nullptr);
        return window;
    }

    void GLFWProxy::destroyWindow(GLFWwindow* window)
    {
        glfwDestroyWindow(window);
    }

    void GLFWProxy::showWindow(GLFWwindow* window)
    {
        glfwShowWindow(window);
    }

    void GLFWProxy::hideWindow(GLFWwindow* window)
    {
        glfwHideWindow(window);
    }

    void GLFWProxy::pollEvents()
    {
        glfwPollEvents();
    }

    void GLFWProxy::setWindowUserData(GLFWwindow* window, void* data)
    {
        glfwSetWindowUserPointer(window, data);
    }

    void* GLFWProxy::getWindowUserData(GLFWwindow* window)
    {
        return glfwGetWindowUserPointer(window);
    }

    void GLFWProxy::setCallbacks(GLFWwindow* window, const GLFWWindowCallbacks& callbacks)
    {
        glfwSetWindowSizeCallback(window, callbacks.resizeCallback);
        glfwSetWindowCloseCallback(window, callbacks.closeCallback);
        glfwSetKeyCallback(window, callbacks.keyCallback);
        glfwSetCharCallback(window, callbacks.charCallback);
        glfwSetMouseButtonCallback(window, callbacks.mouseButtonCallback);
        glfwSetScrollCallback(window, callbacks.scrollCallback);
        glfwSetCursorPosCallback(window, callbacks.cursorPosCallback);
        glfwSetCursorEnterCallback(window, callbacks.cursorEnterCallback);
        glfwSetFramebufferSizeCallback(window, callbacks.framebufferSizeCallback);
        glfwSetDropCallback(window, callbacks.dropCallback);
    }
}

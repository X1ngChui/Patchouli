#pragma once

#include <GLFW/glfw3.h>
#include "Window/Window.h"
#undef APIENTRY

namespace Patchouli
{
    struct GLFWWindowCallbacks
    {
        GLFWwindowclosefun closeCallback = nullptr;
        GLFWwindowsizefun resizeCallback = nullptr;
        GLFWkeyfun keyCallback = nullptr;
        GLFWcharfun charCallback = nullptr;
        GLFWmousebuttonfun mouseButtonCallback = nullptr;
        GLFWscrollfun scrollCallback = nullptr;
        GLFWcursorposfun cursorPosCallback = nullptr;
        GLFWcursorenterfun cursorEnterCallback = nullptr;
        GLFWframebuffersizefun framebufferSizeCallback = nullptr;
        GLFWdropfun dropCallback = nullptr;
    };

    class GLFWProxy
    {
    public:
        GLFWProxy(const GLFWProxy&) = delete;
        GLFWProxy& operator=(const GLFWProxy&) = delete;

        static GLFWProxy& getInstance();

        GLFWwindow* createWindow(const WindowCreateInfo& info);
        void destroyWindow(GLFWwindow* window);
        void showWindow(GLFWwindow* window);
        void hideWindow(GLFWwindow* window);
        void pollEvents();
        void setWindowUserData(GLFWwindow* window, void* data);
        void* getWindowUserData(GLFWwindow* window);
        void setCallbacks(GLFWwindow* window, const GLFWWindowCallbacks& callbacks);

    private:
        GLFWProxy();
        ~GLFWProxy();

        inline static constexpr int test(WindowOptionFlags flags, WindowOptionFlagBits bit)
        {
            return (flags & bit) ? GLFW_TRUE : GLFW_FALSE;
        }
    };
}
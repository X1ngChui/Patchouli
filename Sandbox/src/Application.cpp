#include "Application.h"
#include <iostream>
using namespace Patchouli; // Using Patchouli namespace for convenience

int main()
{
    auto app = Sandbox::Application::getInstance(); // Get the instance of the application
    app->run(); // Run the application
}

namespace Sandbox
{
    // Constructor
    Application::Application()
    {
#ifdef SANDBOX_CONSOLE_ENABLE
        // Initialize console logging
        Console::init("Sandbox", LogLevel::Trace);
        Console::info("Hello Patchouli!");
#endif

        // Create window with specified parameters
        WindowCreateInfo windowCreateInfo = {
            .windowAPI = WindowAPI::GLFW,
            .windowTitle = "Patchouli",
            .windowSize = { 1280, 720 },
            .flags = WindowResizable | WindowDecorated,
            .windowEventCallback = [this](const Event& event) { onEvent(event); }
        };
        window = Window::create(windowCreateInfo);

        // Create graphics context with specified parameters
        GraphicsContextCreateInfo graphicsCreateInfo = {
            .graphicsAPI = GraphicsAPI::Vulkan,
            .graphicsPolicy = GraphicsPolicy::PerformancePriority,
            .window = window,
            .deviceSelector = [](const Ref<GraphicsDevice> devices[], std::size_t size) {
                assert(devices != nullptr && size > 0);
                for (std::size_t i = 0; i < size; i++)
                    if (devices[i]->getProperties().discreteGPU)
                        return devices[i];
                return devices[0];
            }
        };
        graphicsContext = GraphicsContext::create(graphicsCreateInfo);
    }

    // Destructor
    Application::~Application()
    {
    }

    // Method called when the application is updated
    void Application::onUpdate()
    {
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> timeElapsed = currentTime - lastUpdateTime;
        double interval = timeElapsed.count();
        Console::info("Time interval since last update: {} milliseconds", interval);
        lastUpdateTime = currentTime;

        window->onUpdate();
    }

    void Application::onEvent(const Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<WindowCloseEvent>([this](const WindowCloseEvent& e) -> bool
            {
                running = false;
                return true;
            }
        );
    }

    // Method to start the application
    void Application::run()
    {
        running = true;
        window->show(); // Show the window
        while (running)
            onUpdate();
    }

    // Method to get the instance of the application (singleton pattern)
    Ref<Application> Application::getInstance()
    {
        static Application app; // Create a static instance of the application
        return std::static_pointer_cast<Application>(app.shared_from_this()); // Return a shared pointer to the application instance
    }
}

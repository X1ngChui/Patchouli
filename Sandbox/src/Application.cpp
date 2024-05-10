#include "Application.h"

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

        // Set up event handlers
        handlers.onWindowClose = makeRef<EventHandler<WindowCloseEvent>>(
            [this](Ref<Event> event) {
                manager.publish<TerminationEvent>();
            } // Event handler for window close event
        );

        handlers.onAppUpdate = makeRef<EventHandler<AppUpdateEvent>>(
            [this](Ref<Event> event) { this->onUpdate(); }
        ); // Event handler for application update event

        handlers.onInput = makeRef<EventHandler<PATCHOULI_EVENT_TOPIC_MOUSE, PATCHOULI_EVENT_TOPIC_KEYBOARD>>(
            [](Ref<Event> event) { Console::info(*event); }
        );

        // Add event handlers to the manager
        manager.addHandler(handlers.onWindowClose)
            .addHandler(handlers.onAppUpdate)
            .addHandler(handlers.onInput);

        // Create window with specified parameters
        WindowCreateInfo windowCreateInfo = {
            .windowAPI = WindowAPI::GLFW,
            .windowTitle = "Patchouli",
            .windowWidth = 1280,
            .windowHeight = 720,
            .windowEventCallback = [this](Ref<Event> event) { manager.publish(event); }
        };
        window = Window::create(windowCreateInfo);

        // Create graphics context with specified parameters
        GraphicsContextCreateInfo graphicsCreateInfo = {
            .graphicsAPI = GraphicsAPI::Vulkan,
            .graphicsPolicy = GraphicsPolicy::PerformancePriority,
            .window = window,
            .deviceSelector = [](const std::vector<Ref<GraphicsDevice>>& devices) {
            for (Ref<GraphicsDevice> device : devices)
                if (device->getProperties().discreteGPU)
                    return device;
            return devices[0];
            }
        };
        graphicsContext = GraphicsContext::create(graphicsCreateInfo);
    }

    // Destructor
    Application::~Application()
    {
        // Remove event handlers from the manager
        manager.removeHandler(handlers.onWindowClose)
            .removeHandler(handlers.onAppUpdate)
            .removeHandler(handlers.onInput);
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
        manager.publish<AppUpdateEvent>();
    }

    // Method to start the application
    void Application::run()
    {
        window->show(); // Show the window
        manager.publish<AppUpdateEvent>(); // Publish an application update event
        manager.run(); // Run the event manager
    }

    // Method to get the instance of the application (singleton pattern)
    Ref<Application> Application::getInstance()
    {
        static Application app; // Create a static instance of the application
        return std::static_pointer_cast<Application>(app.shared_from_this()); // Return a shared pointer to the application instance
    }
}

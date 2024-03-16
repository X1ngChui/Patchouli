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
        Console::init("Sandbox");
        Console::info("Hello Patchouli!");
#endif
        // Create window with specified parameters
        WindowCreateInfo windowCreateInfo = {
            .windowAPI = WindowAPI::GLFW,
            .windowTitle = "Patchouli",
            .windowWidth = 1280,
            .windowHeight = 720,
            .windowEventCallback = [this](Ref<Event> event) { dispatcher.publishEvent(event); } // Callback function for window events
        };
        window = Window::create(windowCreateInfo);

        // Create graphics context with specified parameters
        GraphicsCreateInfo graphicsCreateInfo = {
            .graphicsAPI = GraphicsAPI::Vulkan,
            .graphicsPolicy = GraphicsPolicy::PerformancePriority,
            .window = window
        };
        graphicsContext = GraphicsContext::create(graphicsCreateInfo);

        // Set up event listeners
        listeners.onWindowClose = makeRef<EventListener<WindowCloseEvent>>(
            [this](Ref<Event> event) { dispatcher.stop(); } // Event listener for window close event
        );

        listeners.onAppUpdate = makeRef<EventListener<AppUpdateEvent>>(
            [this](Ref<Event> event) { this->onUpdate(); dispatcher.publishEvent(makeRef<AppUpdateEvent>()); }, // Event listener for application update event
            EventListenerBase::ExecutionThread::Main // Execute event listener in the main thread
        );

        // Add event listeners to the dispatcher
        dispatcher.addListener(listeners.onWindowClose);
        dispatcher.addListener(listeners.onAppUpdate);
    }

    // Method called when the application is updated
    void Application::onUpdate()
    {
        window->onUpdate(); // Update the window
    }

    // Destructor
    Application::~Application()
    {
    }

    // Method to start the application
    void Application::run()
    {
        window->show(); // Show the window
        dispatcher.publishEvent(makeRef<AppUpdateEvent>()); // Publish an application update event
        dispatcher.run(); // Run the event dispatcher
    }

    // Method to get the instance of the application (singleton pattern)
    Ref<Application> Application::getInstance()
    {
        static Application app; // Create a static instance of the application
        return std::static_pointer_cast<Application>(app.shared_from_this()); // Return a shared pointer to the application instance
    }
}

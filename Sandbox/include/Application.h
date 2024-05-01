#pragma once

#ifdef SANDBOX_DEBUG_MODE
#   define PATCHOULI_CONSOLE_ENABLE
#   define SANDBOX_CONSOLE_ENABLE
#endif

#include "Patchouli.h"

namespace Sandbox
{
    // Class representing the application
    class Application : public Patchouli::PObject
    {
    public:
        // Constructor
        Application();

        // Destructor
        virtual ~Application();

        // Delete copy constructor and assignment operator to prevent copying
        Application(const Application& other) = delete;
        Application& operator=(const Application& other) = delete;

        // Method to start the application
        void run();

        // Method to get the instance of the application (singleton pattern)
        static Patchouli::Ref<Application> getInstance();

    private:
        // Method called when the application is updated
        void onUpdate();

    private:
        // Structure holding event handlers for the application
        struct EventHandlerGroup
        {
            // Event handler for window close event
            Patchouli::Ref<Patchouli::EventHandler<Patchouli::WindowCloseEvent>> onWindowClose = nullptr;

            // Event handler for application update event
            Patchouli::Ref<Patchouli::EventHandler<Patchouli::AppUpdateEvent>> onAppUpdate = nullptr;

            // Evnet handler for input event
            Patchouli::Ref<Patchouli::EventHandler<PATCHOULI_EVENT_TOPIC_KEYBOARD, PATCHOULI_EVENT_TOPIC_MOUSE>> onInput = nullptr;
        };

        EventHandlerGroup handlers; // Group of event handler
        Patchouli::EventManager manager; // Event manager for managing events

        Patchouli::Ref<Patchouli::Window> window = nullptr; // Pointer to the application window
        Patchouli::Ref<Patchouli::GraphicsContext> graphicsContext = nullptr; // Pointer to the graphics context

        std::chrono::steady_clock::time_point lastUpdateTime;
    };
}

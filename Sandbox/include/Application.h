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
        // Structure holding event listeners for the application
        struct EventListenerGroup
        {
            Patchouli::Ref<Patchouli::EventListener<Patchouli::WindowCloseEvent>> onWindowClose = nullptr; // Event listener for window close event
            Patchouli::Ref<Patchouli::EventListener<Patchouli::AppUpdateEvent>> onAppUpdate = nullptr;   // Event listener for application update event
        };
        EventListenerGroup listeners; // Group of event listeners
        Patchouli::EventDispatcher dispatcher; // Event dispatcher for managing events

        Patchouli::Ref<Patchouli::Window> window = nullptr;                   // Pointer to the application window
        Patchouli::Ref<Patchouli::GraphicsContext> graphicsContext = nullptr; // Pointer to the graphics context
    };
} // namespace Sandbox

#pragma once

// #ifdef SANDBOX_DEBUG_MODE
#if defined(SANDBOX_DEBUG_MODE) || defined(SANDBOX_TESTING_MODE)
#   define PATCHOULI_CONSOLE_ENABLE
#   define SANDBOX_CONSOLE_ENABLE
#endif

#include "Patchouli.h"

namespace Sandbox
{
    // Class representing the application
    class Application : public Patchouli::RefBase<Application>
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
        static Patchouli::BorRef<Application> getInstance();

    private:
        // Method called when the application is updated
        void onUpdate();

        // Method called when the application is updated
        void onEvent(const Patchouli::Event& event);

    private:
        bool running = false;

        Patchouli::Ref<Patchouli::Window> window = nullptr; // Pointer to the application window
        Patchouli::Ref<Patchouli::GraphicsContext> graphicsContext = nullptr; // Pointer to the graphics context

        std::chrono::steady_clock::time_point lastUpdateTime;
    };
}

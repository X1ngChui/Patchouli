#pragma once
#include "Core/Base.h"
#include "Graphics/GraphicsObject.h"
#include "Graphics/GraphicsDevice.h"

namespace Patchouli
{
    // Enum representing different windowing APIs
    enum class WindowAPI
    {
        None = 0,   // No window
        GLFW        // GLFW windowing API
    };

    // Structure holding graphics-related information
    struct GraphicsInfo
    {
        const char* appName;    // Name of the application
        uint32_t appVersion;    // Version of the application
        WindowAPI windowAPI;    // Windowing API used by the application
    };

    // Abstract base class representing a graphics context
    class PATCHOULI_API GraphicsContext : public GraphicsObject
    {
    public:
        GraphicsContext() = default;
        virtual ~GraphicsContext() = default;

        // Function to retrieve graphics devices
        virtual std::vector<Ref<GraphicsDevice>> getDevices() const = 0;

        // Function to select a graphics device
        virtual void selectDevice(Ref<GraphicsDevice> device) = 0;

        // Static function to create a graphics context
        static Ref<GraphicsContext> create(const GraphicsInfo& info);
    };
}

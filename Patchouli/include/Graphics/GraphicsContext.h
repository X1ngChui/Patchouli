#pragma once
#include "Core/Base.h"
#include "Window/Window.h"
#include "Graphics/GraphicsObject.h"
#include "Graphics/GraphicsDevice.h"

namespace Patchouli
{
    // Enum representing different graphics APIs
    enum class GraphicsAPI
    {
        None = 0,
        Vulkan
    };

    // Structure holding graphics-related information
    struct GraphicsInfo
    {
        GraphicsAPI graphicsAPI = GraphicsAPI::Vulkan;
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

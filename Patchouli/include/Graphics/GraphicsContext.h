#pragma once
#include "Core/Base.h"
#include "Core/PObject.h"
#include "Window/Window.h"
#include "Graphics/GraphicsDevice.h"

namespace Patchouli
{
    // Enum representing different graphics APIs
    enum class GraphicsAPI
    {
        None = 0,
        Vulkan
    };

    enum class GraphicsPolicy
    {
        PerformancePriority = 0,
        PowerSavingPriority
    };

    // Structure holding graphics-related information
    struct GraphicsCreateInfo
    {
        GraphicsAPI graphicsAPI = GraphicsAPI::Vulkan;
        GraphicsPolicy graphicsPolicy = GraphicsPolicy::PerformancePriority;

        Ref<Window> window;
    };

    // Abstract base class representing a graphics context
    class PATCHOULI_API GraphicsContext : public PObject
    {
    public:
        GraphicsContext() = default;
        virtual ~GraphicsContext() = default;

        // Function to retrieve graphics devices
        virtual std::vector<Ref<GraphicsDevice>> getDevices() const = 0;

        // Function to select a graphics device
        virtual void selectDevice(Ref<GraphicsDevice> device) = 0;

        // Static function to create a graphics context
        static Ref<GraphicsContext> create(const GraphicsCreateInfo& info);
    };
}

#pragma once
#include "Core/Base.h"
#include "Window/Window.h"
#include "Graphics/GraphicsDevice.h"

namespace Patchouli
{
    // Enum representing different graphics APIs
    enum class GraphicsAPI
    {
        Vulkan = 0
    };

    enum class GraphicsPolicy
    {
        PerformancePriority = 0,
        PowerSavingPriority
    };

    // Structure holding graphics-related information
    struct GraphicsContextCreateInfo
    {
        GraphicsAPI graphicsAPI = GraphicsAPI::Vulkan;
        GraphicsPolicy graphicsPolicy = GraphicsPolicy::PerformancePriority;

        Ref<Window> window;

        using DeviceSelector = std::function<Ref<GraphicsDevice>(const Ref<GraphicsDevice>[], std::size_t)>;
        DeviceSelector deviceSelector;
    };

    // Abstract base class representing a graphics context
    class PATCHOULI_API GraphicsContext : public RefBase<GraphicsContext>
    {
    public:
        GraphicsContext() = default;
        virtual ~GraphicsContext() = default;

        // Static function to create a graphics context
        static Ref<GraphicsContext> create(const GraphicsContextCreateInfo& info);
    };
}

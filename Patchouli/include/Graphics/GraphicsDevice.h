#pragma once

#include "Core/Base.h"
#include "Graphics/GraphicsObject.h"

#define PATCHOULI_MAX_DEVICE_NAME_SIZE (256)

namespace Patchouli
{
    // Structure to hold properties of a graphics device
    struct GraphicsDeviceProperties
    {
        char deviceName[PATCHOULI_MAX_DEVICE_NAME_SIZE]; // Name of the graphics device
        uint32_t apiVersion;        // API version supported by the device
        uint32_t driverVersion;     // Driver version of the device
        uint32_t vendorID;          // Vendor ID of the device
        uint32_t deviceID;          // Device ID of the device
        bool discreteGPU;           // Flag indicating if the device is a discrete GPU
    };

    // Structure to hold features supported by a graphics device
    struct GraphicsDeviceFeatures
    {
        bool geometryShader;        // Flag indicating if geometry shader is supported
    };

    // Class representing a graphics device
    class PATCHOULI_API GraphicsDevice : public GraphicsObject
    {
    public:
        virtual ~GraphicsDevice() = default;

        // Getter functions for GraphicsDeviceProperties
        const GraphicsDeviceProperties& getProperties() const { return properties; }

        // Getter functions for GraphicsDeviceFeatures
        const GraphicsDeviceFeatures& getFeatures() const { return features; }

    protected:
        GraphicsDeviceProperties properties;    // Properties of the graphics device
        GraphicsDeviceFeatures features;        // Features supported by the graphics device
    };
}

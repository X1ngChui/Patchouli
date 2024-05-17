#pragma once

#include "Core/Base.h"
#include "Core/PObject.h"

#define PATCHOULI_DEVICE_NAME_SIZE 256
namespace Patchouli
{
    // Structure to hold properties of a graphics device
    struct GraphicsDeviceProperties
    {
        char name[PATCHOULI_DEVICE_NAME_SIZE]; // Name of the graphics device
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
    class PATCHOULI_API GraphicsDevice : public PObject
    {
    public:
        virtual ~GraphicsDevice() = default;

        // Getter functions for GraphicsDeviceProperties
        virtual GraphicsDeviceProperties getProperties() const = 0;

        // Getter functions for GraphicsDeviceFeatures
        virtual GraphicsDeviceFeatures getFeatures() const = 0;
    };
}

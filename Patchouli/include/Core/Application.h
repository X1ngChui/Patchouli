#pragma once

#include "Core/Base.h"
#include "Core/Layer.h"
#include "Window/Window.h"
#include "Graphics/GraphicsContext.h"

#define PATCHOULI_SUBSYSTEM_FLAG(x) (1 << (x))
#define PATCHOULI_SUBSYSTEM_ALL (0xffffffff)

namespace Patchouli
{
	/* Function to generate a version */
	static inline constexpr uint32_t version(uint32_t variant, uint32_t major, uint32_t minor, uint32_t patch)
	{
		return (variant << 29U) | (major << 22U) | (minor << 12U) | patch;
	}

	/* Enum representing various subsystems of the application */
	namespace Subsystem
	{
		enum subsystem_t : uint32_t
		{
			None = 0,
			Graphics = PATCHOULI_SUBSYSTEM_FLAG(0),
			All = PATCHOULI_SUBSYSTEM_ALL
		};
	}

	/* Structure representing information about the application */
	struct ApplicationInfo
	{
		const char* appName = "Patchouli";			/* Name of the application */
		const uint32_t appVersion = 0;				/* Version of the application */
		const uint32_t subsystems = 0;				/* Subsystems enabled for the application */
		const WindowInfo windowInfo;
	};

	/* Class representing the application. */
	class PATCHOULI_API Application
	{
	public:
		virtual ~Application() = default;

		/* Function to check if the application is running */
		bool isRunning() const { return this->running; }

		/* Function to start running the application */
		void run();

		void onEvent(Event* event);

		const ApplicationInfo& getAppInfo() const { return appInfo; }

		static const Application& getInstance() { return *instance; }

	protected:
		Application(const ApplicationInfo& info);

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void pushOverlay(Overlay* overlay);
		void popOverlay(Overlay* overlay);
	private:
		/* Function to initialize the application and its subsystems */
		void init();

		/* Function to initialize the graphics subsystem */
		void graphicsInit(const ApplicationInfo& appInfo);
	private:
		bool running;								/* Flag indicating whether the application is running */
		ApplicationInfo appInfo;					/* Information about the application */
		LayerStack layerStack;						/* Layers container */
		Ref<Window> window = nullptr;				/* Window pointer */

		inline static Application* instance = nullptr;
	};
}

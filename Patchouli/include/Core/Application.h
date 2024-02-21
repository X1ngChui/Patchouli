#pragma once

#include "Core/PObject.h"

namespace Patchouli
{
	/* Function to generate a version */
	static inline constexpr uint32_t version(uint32_t variant, uint32_t major, uint32_t minor, uint32_t patch)
	{
		return (variant << 29U) | (major << 22U) | (minor << 12U) | patch;
	}

	/* Enum representing various subsystems of the application */
	enum Subsystem : uint32_t
	{
		None = 0x00000000,
		Logging = 0x00000001
	};

	/* Structure representing information about the application */
	struct ApplicationInfo
	{
		const char* appName = "Patchouli";			/* Name of the application */
		uint32_t version = 0;						/* Version of the application */
		uint32_t subsystems = Subsystem::None;		/* Subsystems enabled for the application */
	};

	/* Class representing the application. */
	class Application : public PObject
	{
	public:
		virtual ~Application() = default;

		/* Function to check if the application is running */
		bool isRunning() const { return this->running; }

		/* Function to start running the application */
		void run();

	protected:
		Application(const ApplicationInfo& info);

	private:
		/* Function to initialize the application and its subsystems */
		void init();

	private:
		bool running;								/* Flag indicating whether the application is running */
		ApplicationInfo appInfo;					/* Information about the application */
	};
}
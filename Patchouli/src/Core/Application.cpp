#include "Core/Application.h"
#include "Log/Console.h"

namespace Patchouli
{
	Application::Application(const ApplicationInfo& info)
		: running(false), appInfo(info)
	{
		init();
	}

	void Application::run()
	{
		running = true;

		while (running);
	}

	void Application::init()
	{
		uint32_t subsystems = appInfo.subsystems;
		while (subsystems)
		{
			// Extract the lowest bit representing a subsystem
			uint32_t subsys = subsystems & (-subsystems);
			// Clear the lowest bit representing the subsystem
			subsystems &= ~subsys;

			// Initialize each subsystem based on the extracted subsystem bit.
			switch (subsys)
			{
			case Subsystem::Logging:
				Console::init(appInfo.appName);
				Console::coreInfo("Hello Patchouli!");
				break;
			}
		}
	}
}
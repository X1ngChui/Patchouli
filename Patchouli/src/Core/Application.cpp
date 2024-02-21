#include "Core/Application.h"
#include "Log/Console.h"

namespace Patchouli
{
	Ref<Application> Application::instance = nullptr;

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
#ifdef PATCHOULI_CONSOLE_ENABLE
		Console::init(appInfo.appName);
		Console::coreInfo("Hello Patchouli!");
#endif
	}
}
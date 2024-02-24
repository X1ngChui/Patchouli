#include "Sandbox.h"

namespace Sandbox
{
	Sandbox::Sandbox(const Patchouli::ApplicationInfo& info)
		: Application(info)
	{
	}
}

int main()
{
	Patchouli::ApplicationInfo info = {
		.appName = "Sandbox",
		.version = Patchouli::version(0, 0, 0, 0),
		.subsystems = Patchouli::Subsystem::Logging
	};

	auto* app = new Sandbox::Sandbox(info);

	Patchouli::Console::info("{0} = {1}.", "Cirno", 9);
	Patchouli::Logger logger("logger", "log.log", Patchouli::LogLevel::Warn);
	logger.info("Hello Patchouli!");

	app->run();
	delete app;
}
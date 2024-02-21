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

	Sandbox::Sandbox* app = new Sandbox::Sandbox(info);

	app->run();
}
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

	Patchouli::Ref<Sandbox::Sandbox> app = Patchouli::makeRef<Sandbox::Sandbox>(info);
	Patchouli::Console::info("This is Sandbox.");
	app->run();
}
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
	constexpr Patchouli::ApplicationInfo info = {
		.appName = "Sandbox",
		.appVersion = Patchouli::version(0, 0, 0, 0),
		.subsystems = Patchouli::Subsystem::Graphics
	};

	auto* app = new Sandbox::Sandbox(info);
	app->run();
	delete app;
}
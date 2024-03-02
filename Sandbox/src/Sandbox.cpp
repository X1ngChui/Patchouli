#include "Sandbox.h"
#include <iostream>

namespace Sandbox
{
	Sandbox::Sandbox(const Patchouli::ApplicationInfo& info)
		: Application(info)
	{
		Patchouli::GraphicsInfo graphicsInfo = {
			.appName = info.appName,
			.appVersion = info.appVersion,
			.windowAPI = Patchouli::WindowAPI::GLFW
		};

		graphicsContext = Patchouli::GraphicsContext::create(graphicsInfo);
		auto devices = graphicsContext->getDevices();
		graphicsContext->selectDevice(devices[0]);
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
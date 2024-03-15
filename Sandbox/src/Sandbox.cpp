#include "Sandbox.h"
#include <iostream>

namespace Sandbox
{
	Sandbox::Sandbox(const Patchouli::ApplicationInfo& info)
		: Application(info)
	{
		const Patchouli::GraphicsInfo graphicsInfo = {
			.graphicsAPI = Patchouli::GraphicsAPI::Vulkan,
			.graphicsPolicy = Patchouli::GraphicsPolicy::PerformancePriority
		};
		graphicsContext = Patchouli::GraphicsContext::create(graphicsInfo);
		auto devices = graphicsContext->getDevices();
		graphicsContext->selectDevice(devices[0]);
	}
}

int main()
{
	Patchouli::WindowInfo windowInfo = {
		.windowAPI = Patchouli::WindowAPI::GLFW,
		.windowTitle = "Sandbox",
		.windowWidth = 1280,
		.windowHeight = 720
	};

	Patchouli::ApplicationInfo appInfo = {
		.appName = "Sandbox",
		.appVersion = Patchouli::version(0, 0, 0, 0),
		.subsystems = Patchouli::Subsystem::Graphics,
		.windowInfo = windowInfo
	};

	auto app = Patchouli::makeRef<Sandbox::Sandbox>(appInfo);
	app->run();
}
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
		// Initialize the internal subsystem
		Console::coreInit();

		// Initialize the custom subsystem
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
				break;
			}
		}
	}

	void Application::pushLayer(Layer* layer)
	{
		layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::popLayer(Layer* layer)
	{
		layerStack.popLayer(layer);
		layer->onDetach();
	}

	void Application::pushOverlay(Overlay* overlay)
	{
		layerStack.pushOverlay(overlay);
		overlay->onAttach();
	}

	void Application::popOverlay(Overlay* overlay)
	{
		layerStack.popOverlay(overlay);
		overlay->onDetach();
	}
}
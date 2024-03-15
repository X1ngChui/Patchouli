#include "Core/Application.h"
#include "Log/Console.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"

namespace Patchouli
{
	Application::Application(const ApplicationInfo& info)
		: running(false), appInfo(info)
	{
		assert(instance == nullptr);
		instance = this;
		init();

		eventDispatcher = makeRef<EventDispatcher>();
		listener = makeRef<EventListener<WindowCloseEvent>>( [this](Event& event) { running = false; });
		eventDispatcher->addListener(listener);
	}

	void Application::run()
	{
		running = true;

		window->show();

		while (running)
		{
			window->onUpdate();
		}
	}

	void Application::init()
	{
		// Initialize the console subsystem for debugging
#ifdef PATCHOULI_CONSOLE_ENABLE
		Console::init(appInfo.appName);
		Console::coreInfo("Hello {}!", appInfo.appName);
#endif

		// Initialize the custom subsystem
		uint32_t subsystems = appInfo.subsystems;
		while (subsystems)
		{
			// Extract the lowest bit representing a subsystem
			uint32_t subsys = subsystems & (-subsystems);
			// Clear the lowest bit representing the subsystem
			subsystems &= ~subsys;

			// Initialize each subsystem based on the extracted subsystem bit
			switch (subsys)
			{
			case Subsystem::Graphics:
				graphicsInit(appInfo);
				break;
			}
		}
	}

	void Application::graphicsInit(const ApplicationInfo& appInfo)
	{
		window = Window::create(appInfo.windowInfo);
		window->setEventCallback([this](Event& event) { eventDispatcher->dispatch(event); });
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
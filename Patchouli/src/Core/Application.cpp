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
		init();
	}

	void Application::run()
	{
		running = true;

		while (running)
		{
			window->onUpdate();
		}
	}

	void Application::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& event) -> bool {
			this->running = false;
			return true;
		});
	}

	void Application::init()
	{
		// Initialize the console subsystem for debugging'
#ifdef PATCHOULI_CONSOLE_ENABLE
		Console::init(appInfo.appName);
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
				WindowInfo windowInfo = {
					.title = appInfo.appName,
					.width = 1920,
					.height = 1080
				};
				window = Window::create(windowInfo);
				window->setEventCallback([this](Event* event) { this->onEvent(*event); });

				GraphicsInfo graphicsInfo = {
					.appName = appInfo.appName,
					.appVersion = appInfo.appVersion,
					.windowAPI = WindowAPI::GLFW
				};
				graphicsContext = GraphicsContext::create(graphicsInfo);
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
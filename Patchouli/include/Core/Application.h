#pragma once

#include "Core/PObject.h"

namespace Patchouli
{
	static inline uint32_t version(uint32_t variant, uint32_t major, uint32_t minor, uint32_t patch)
	{
		return (variant << 29U) | (major << 22U) | (minor << 12U) | patch;
	}

	struct ApplicationInfo
	{
		const char* appName = "Patchouli";
		uint32_t version = 0;
	};

	class Application : public PObject
	{
	public:
		virtual ~Application() = default;

		bool isRunning() const { return this->running; }

		void run();

	protected:
		Application(const ApplicationInfo& info);

	private:
		void init();

	private:
		bool running;
		ApplicationInfo appInfo;

		static Ref<Application> instance;
	};
}
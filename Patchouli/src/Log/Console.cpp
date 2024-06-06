#include "Log/Console.h"

namespace Patchouli
{
	void Console::init(const char* appName, const LogLevel level)
	{
#ifdef PATCHOULI_CONSOLE_ENABLE
		coreLogger = spdlog::stdout_color_mt(CORE_CONSOLE_NAME);
		coreLogger->set_pattern(CONSOLE_FORMAT);
		coreLogger->set_level((spdlog::level::level_enum)(level));

		clientLogger = spdlog::stdout_color_mt(appName);
		clientLogger->set_pattern(CONSOLE_FORMAT);
		clientLogger->set_level((spdlog::level::level_enum)(level));
#endif
	}

	void Console::setLevel(const LogLevel level)
	{
#ifdef PATCHOULI_CONSOLE_ENABLE
		clientLogger->set_level((spdlog::level::level_enum)(level));
#endif
	}

	void Console::setCoreLevel(const LogLevel level)
	{
#ifdef PATCHOULI_CONSOLE_ENABLE
		coreLogger->set_level((spdlog::level::level_enum)(level));
#endif
	}
}
#include "Log/Console.h"

namespace Patchouli
{
	void Console::init(const char* name, const LogLevel level)
	{
#ifdef PATCHOULI_CONSOLE_ENABLE
		clientLogger = spdlog::stdout_color_mt(name);
		clientLogger->set_pattern("%^[%T][%n][%l] %v%$");
		clientLogger->set_level((spdlog::level::level_enum)(level));
#else
		(void)(name);
#endif
	}

	void Console::setLevel(const LogLevel level)
	{
		clientLogger->set_level((spdlog::level::level_enum)(level));
	}

	void Console::setCoreLevel(const LogLevel level)
	{
		coreLogger->set_level((spdlog::level::level_enum)(level));
	}
}
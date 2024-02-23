#include "Log/Console.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#ifdef max
	#undef max
#endif

namespace Patchouli
{
	std::shared_ptr<spdlog::logger> Console::coreLogger;
	std::shared_ptr<spdlog::logger> Console::clientLogger;

	void Console::init(const char* name)
	{
#ifdef PATCHOULI_CONSOLE_ENABLE
		spdlog::set_pattern("%^[%T] [%n] [%l] %v%$");
		coreLogger = spdlog::stdout_color_mt("Patchouli");
		clientLogger = spdlog::stdout_color_mt(name);
#else
		(void)(name);
#endif
	}
}
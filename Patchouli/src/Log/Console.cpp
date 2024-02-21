#include "Log/Console.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Patchouli
{
	std::shared_ptr<spdlog::logger> Console::coreLogger;
	std::shared_ptr<spdlog::logger> Console::clientLogger;

	void Console::init(const char* name)
	{
		spdlog::set_pattern("%^[%T] [%l] [%n] %v%$");
		coreLogger = spdlog::stdout_color_mt("Patchouli");
		clientLogger = spdlog::stdout_color_mt(name);
	}
}
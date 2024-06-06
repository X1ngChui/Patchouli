#include "Log/Logger.h"

#define CONSOLE_FORMAT ("[%T][%n][%l] %v")

namespace Patchouli
{
	Logger::Logger(const char* name, std::filesystem::path path, LogLevel level)
		: logger(spdlog::basic_logger_mt(name, path.generic_string()))
	{
		logger->set_pattern(CONSOLE_FORMAT);
		logger->set_level((spdlog::level::level_enum)(level));
	}

	void Logger::setLevel(const LogLevel level)
	{
		logger->set_level((spdlog::level::level_enum)(level));
	}
}
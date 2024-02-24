#include "Log/Logger.h"

namespace Patchouli
{
	Logger::Logger(const char* name, std::filesystem::path path, LogLevel level)
		: logger(spdlog::basic_logger_mt(name, path.generic_string()))
	{
		logger->set_pattern("[%T][%n][%l] %v");
		logger->set_level((spdlog::level::level_enum)(level));
	}

	void Logger::setLevel(const LogLevel level)
	{
		logger->set_level((spdlog::level::level_enum)(level));
	}
}
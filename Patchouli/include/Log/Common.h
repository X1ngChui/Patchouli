#pragma once

#include <spdlog/spdlog.h>

namespace Patchouli
{
	enum class LogLevel
	{
		Trace = spdlog::level::trace,
		Info = spdlog::level::info,
		Warn = spdlog::level::warn,
		Error = spdlog::level::err,
		Critical = spdlog::level::critical
	};
}
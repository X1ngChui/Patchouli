#pragma once

#include "PatchouliPch.h"
#include "Core/Base.h"
#include "Log/Common.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Patchouli
{
	class PATCHOULI_API Logger
	{
	public:
		Logger(const char* name, std::filesystem::path path, LogLevel level = LogLevel::Info);

		~Logger() = default;

		void setLevel(const LogLevel level);

		template <typename... Args>
		void trace(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			logger->trace(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		void trace(T&& msg)
		{
			logger->trace(std::forward<T>(msg));
		}

		template <typename... Args>
		void info(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			logger->info(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		void info(T&& msg)
		{
			logger->info(std::forward<T>(msg));
		}

		template <typename... Args>
		void warn(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			logger->warn(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		void warn(T&& msg)
		{
			logger->warn(std::forward<T>(msg));
		}

		template <typename... Args>
		void error(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			logger->error(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		void error(T&& msg)
		{
			logger->error(std::forward<T>(msg));
		}

		template <typename... Args>
		void critical(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			logger->critical(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		void critical(T&& msg)
		{
			logger->critical(std::forward<T>(msg));
		}

#ifdef assert
	#undef assert
#endif
		template <typename... Args>
		void assert(bool assertion, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			if (!assertion)
				logger->error(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		void assert(bool assertion, T&& msg)
		{
			if (!assertion)
				logger->error(std::forward<T>(msg));
		}

	private:
		std::shared_ptr<spdlog::logger> logger;
	};
}
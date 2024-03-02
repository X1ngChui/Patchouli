#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Core/Base.h"
#include "Log/Common.h"

#ifdef _MSC_VER
	#define trap __debugbreak
#endif
#if defined(__GNUC__) || defined(__clang__)
	#define trap __builtin_trap;
#endif

#ifdef PATCHOULI_DEBUG_MODE
	#define PATCHOULI_CONSOLE_ENABLE
#endif

namespace Patchouli
{
	/*
	 * @brief A utility class for logging messages using the spdlog library.
	 */
	class PATCHOULI_API Console
	{
	// Example of using logging functions:
	// Console::coreError("Cirno = 9");								(Output: Cirno = 9)
	// Console::coreInfo("{0} = {1}.", "Cirno", 9);					(Output: Cirno = 9)
	public:
		Console() = delete;
		~Console() = delete;
		
		// Initialization function
		static void init(const char* appName, const LogLevel level = LogLevel::Info);

		// --------------------------------------------------
		// User Logging Functions

		// Functions intended for user usage.
		// These functions should not be used until initialized, and internal usage is discouraged.
		// --------------------------------------------------

		void setLevel(const LogLevel level);

		template <typename... Args>
		static void trace(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->trace(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void trace(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->trace(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void info(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->info(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void info(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->info(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void warn(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->warn(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void warn(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->warn(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void error(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->error(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void error(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->error(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void critical(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->critical(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void critical(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->critical(std::forward<T>(msg));
#endif
		}

		// --------------------------------------------------
		// Core Logging Functions

		// Internal functions that should not be used by users directly.
		// These functions are always available and should only be used internally.
		// --------------------------------------------------

		void setCoreLevel(const LogLevel level);

		template <typename... Args>
		static void coreTrace(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			coreLogger->trace(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void coreTrace(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			coreLogger->trace(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void coreInfo(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			coreLogger->info(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void coreInfo(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			coreLogger->info(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void coreWarn(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			coreLogger->warn(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void coreWarn(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			coreLogger->warn(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void coreError(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			coreLogger->error(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void coreError(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			coreLogger->error(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void coreCritical(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			coreLogger->critical(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void coreCritical(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			coreLogger->critical(std::forward<T>(msg));
#endif
		}

	private:
		inline static std::shared_ptr<spdlog::logger> coreLogger = nullptr;
		inline static std::shared_ptr<spdlog::logger> clientLogger = nullptr;
	};
}
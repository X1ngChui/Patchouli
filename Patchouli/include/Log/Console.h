#pragma once

#include "spdlog/spdlog.h"

#ifdef _MSC_VER
#define trap() __debugbreak()
#endif
#if defined(__GNUC__) || defined(__clang__)
#define trap() __builtin_trap();
#endif

#if defined(PATCHOULI_DEBUG_MODE) || defined(PATCHOULI_TESTING_MODE)
#define PATCHOULI_CONSOLE_ENABLE
#endif

namespace Patchouli
{
	/*
	 * @brief A utility class for logging messages using the spdlog library.
	 */
	class Console
	{
	// Example of using logging functions:
	// Console::coreError("Cirno = 9");								(Output: Cirno = 9)
	// Console::coreInfo("{0} = {1}.", "Cirno", 9);					(Output: Cirno = 9)
	// Console::coreAssert(false, "{0} = {1}.", "Cirno", 9);		(Output: Cirno = 9)
	public:
		static void init(const char* name);

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

		template <typename... Args>
		static void coreAssert(bool assertion, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			if (!assertion)
			{
				coreLogger->error(fmt, std::forward<Args>(args)...);
				trap();
			}
#endif
		}

		template <typename T>
		static void coreAssert(bool assertion, T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			if (!assertion)
			{
				coreLogger->error(std::forward<T>(msg));
				trap();
			}
#endif
		}

		template <typename... Args>
		static void clientTrace(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->trace(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void clientTrace(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->trace(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void clientInfo(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->info(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void clientInfo(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->info(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void clientWarn(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->warn(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void clientWarn(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->warn(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void clientError(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->error(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void clientError(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->error(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void clientCritical(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->critical(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void clientCritical(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			clientLogger->critical(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void clientAssert(bool assertion, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			if (!assertion)
			{
				clientLogger->error(fmt, std::forward<Args>(args)...);
				trap();
			}
#endif
		}

		template <typename T>
		static void clientAssert(bool assertion, T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			if (!assertion)
			{
				clientLogger->error(std::forward<T>(msg));
				trap();
			}
#endif
		}

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};
}
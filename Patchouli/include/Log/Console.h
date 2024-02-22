#pragma once

#include "spdlog/spdlog.h"
#include "Core/Base.h"

#ifdef _MSC_VER
#define trap() __debugbreak()
#endif
#if defined(__GNUC__) || defined(__clang__)
#define trap() __builtin_trap();
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
	// Console::coreAssert(false, "{0} = {1}.", "Cirno", 9);		(Output: Cirno = 9)
	public:
		Console() = delete;
		~Console() = delete;

		static void init(const char* name);

		static std::shared_ptr<spdlog::logger> getCoreLogger();
		static std::shared_ptr<spdlog::logger> getClientLogger();

		template <typename... Args>
		static void coreTrace(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getCoreLogger()->trace(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void coreTrace(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getCoreLogger()->trace(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void coreInfo(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getCoreLogger()->info(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void coreInfo(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getCoreLogger()->info(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void coreWarn(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getCoreLogger()->warn(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void coreWarn(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getCoreLogger()->warn(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void coreError(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getCoreLogger()->error(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void coreError(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getCoreLogger()->error(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void coreCritical(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getCoreLogger()->critical(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void coreCritical(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getCoreLogger()->critical(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void coreAssert(bool assertion, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			if (!assertion)
			{
				getCoreLogger()->error(fmt, std::forward<Args>(args)...);
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
				getCoreLogger()->error(std::forward<T>(msg));
				trap();
			}
#endif
		}

		template <typename... Args>
		static void trace(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getClientLogger()->trace(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void trace(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getClientLogger()->trace(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void info(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getClientLogger()->info(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void info(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getClientLogger()->info(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void warn(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getClientLogger()->warn(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void warn(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getClientLogger()->warn(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void error(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getClientLogger()->error(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void error(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getClientLogger()->error(std::forward<T>(msg));
#endif
		}

		template <typename... Args>
		static void critical(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getClientLogger()->critical(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename T>
		static void critical(T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			getClientLogger()->critical(std::forward<T>(msg));
#endif
		}

#ifdef assert
	#undef assert
#endif
		template <typename... Args>
		static void assert(bool assertion, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			if (!assertion)
			{
				getClientLogger()->error(fmt, std::forward<Args>(args)...);
				trap();
			}
#endif
		}

		template <typename T>
		static void assert(bool assertion, T&& msg)
		{
#ifdef PATCHOULI_CONSOLE_ENABLE
			if (!assertion)
			{
				getClientLogger()->error(std::forward<T>(msg));
				trap();
			}
#endif
		}

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};
}
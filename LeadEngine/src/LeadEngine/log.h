#pragma once

#include <memory>

#include "LeadEngine/core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace le
{ 
	typedef std::shared_ptr<spdlog::logger> logger;

	class LE_API Log
	{
	private:
		static logger coreLogger;
		static logger clientLogger;
	public:
		static void init();

		inline static logger& getCoreLogger()
		{
			return coreLogger;
		}
		inline static logger& getClientLogger()
		{
			return clientLogger;
		}
	
	};
}

/* Core log macros. */
#define LE_CORE_TRACE(...) ::le::Log::getCoreLogger()->trace(__VA_ARGS__)
#define LE_CORE_INFO(...) ::le::Log::getCoreLogger()->info(__VA_ARGS__)
#define LE_CORE_WARN(...) ::le::Log::getCoreLogger()->warn(__VA_ARGS__)
#define LE_CORE_ERROR(...) ::le::Log::getCoreLogger()->error(__VA_ARGS__)
#define LE_CORE_FATAL(...) ::le::Log::getCoreLogger()->fatal(__VA_ARGS__)

/* Client log macros. */
#define LE_TRACE(...) ::le::Log::getClientLogger()->trace(__VA_ARGS__)
#define LE_INFO(...) ::le::Log::getClientLogger()->info(__VA_ARGS__)
#define LE_WARN(...) ::le::Log::getClientLogger()->warn(__VA_ARGS__)
#define LE_ERROR(...) ::le::Log::getClientLogger()->error(__VA_ARGS__)
#define LE_FATAL(...) ::le::Log::getClientLogger()->fatal(__VA_ARGS__)
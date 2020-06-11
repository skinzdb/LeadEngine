#include "le_pch.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace le
{
	logger Log::coreLogger;
	logger Log::clientLogger;

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		coreLogger = spdlog::stdout_color_mt("LEADENGINE");
		coreLogger->set_level(spdlog::level::trace);

		clientLogger = spdlog::stdout_color_mt("APP");
		clientLogger->set_level(spdlog::level::trace);
	}
}
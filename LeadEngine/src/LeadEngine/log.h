#pragma once

#include "core.h"
#include "spdlog/spdlog.h"

namespace le
{
	typedef std::shared_ptr<spdlog::logger> logger;

	class LE_API Log
	{
	private:
		static logger coreLogger;
		static logger clientLogger;
	public:
		Log();
		~Log();

		inline static logger& getCoreLogger()
		{
			return coreLogger;
		}
	
	};
}
#pragma once

#include "core.h"

namespace le
{
	class LE_API App
	{
	public:
		App();
		virtual ~App();

		void run();
	};

	/* Defined in the client. */
	App* create_app();
}




#pragma once

#include "LeadEngine/core.h"
#include "LeadEngine/window.h"

namespace le
{
	class LE_API App
	{
	private:
		std::unique_ptr<Window> window;
		bool running = true;
	public:
		App();
		virtual ~App();

		void run();
	};

	/* Defined in the client. */
	App* create_app();
}




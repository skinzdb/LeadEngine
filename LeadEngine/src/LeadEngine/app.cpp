#include "le_pch.h"

#include "LeadEngine/app.h"

#include "LeadEngine/event.h"

namespace le
{
	App::App()
	{
		window = std::unique_ptr<Window>(Window::create());
	}

	App::~App()
	{
	}

	void App::run()
	{
		while (running)
		{
			window->update();
		}
	}
}


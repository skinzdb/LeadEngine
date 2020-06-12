#include "le_pch.h"

#include "LeadEngine/app.h"

namespace le
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	App::App()
	{
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback(BIND_EVENT_FN(App::onEvent));
	}

	App::~App()
	{
	}

	void App::run()
	{
		while (running)
		{
			window->update();

			for (Layer* layer : layerStack)
				layer->update();
		}
	}

	void App::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(App::onWindowClose));

		LE_CORE_TRACE("{0}", e);

		for (auto i = layerStack.end(); i != layerStack.begin();)
		{
			(*--i)->onEvent(e);
			if (e.handled)
				break;
		}
	}

	void App::pushLayer(Layer* layer)
	{
		layerStack.pushLayer(layer);
	}

	void App::pushOverlay(Layer* overlay)
	{
		layerStack.pushOverlay(overlay);
	}

	bool App::onWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}
}


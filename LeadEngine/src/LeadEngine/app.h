#pragma once

#include "LeadEngine/core.h"
#include "LeadEngine/window.h"
#include "LeadEngine/event.h"
#include "LeadEngine/layer.h"

namespace le
{
	class LE_API App
	{
	private:
		bool onWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> window;
		bool running = true;
		LayerStack layerStack;
	public:
		App();
		virtual ~App();

		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
	};

	/* Defined in the client. */
	App* create_app();
}




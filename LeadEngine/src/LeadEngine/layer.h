#pragma once

#include "LeadEngine/core.h"
#include "LeadEngine/event.h"

namespace le
{
	class LE_API Layer
	{
	private:
		std::string debugName;
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void update() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return debugName; }
	};

	class LE_API LayerStack
	{
	private:
		std::vector<Layer*> layers;
		std::vector<Layer*>::iterator layerInsert;
	public:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer* layer);
		void popOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }
	};
}
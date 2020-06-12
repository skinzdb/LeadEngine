#include "le_pch.h"
#include "layer.h"

namespace le
{
	Layer::Layer(const std::string& debugName) : debugName(debugName)
	{
	}

	Layer::~Layer()
	{
	}

	LayerStack::LayerStack()
	{
		layerInsert = layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : layers)
			delete layer;
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		layerInsert = layers.emplace(layerInsert, layer);
	}
	void LayerStack::pushOverlay(Layer* overlay)
	{
		layers.emplace_back(overlay);
	}
	void LayerStack::popLayer(Layer* layer)
	{
		auto i = std::find(layers.begin(), layers.end(), layer);
		if (i != layers.end())
		{
			layers.erase(i);
			layerInsert--;
		}
	}

	void LayerStack::popOverlay(Layer* overlay)
	{
		auto i = std::find(layers.begin(), layers.end(), overlay);
		if (i != layers.end())
			layers.erase(i);
	}
}


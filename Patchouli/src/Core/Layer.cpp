#include "Core/Layer.h"

namespace Patchouli
{
	Layer::Layer(const char* name)
		: name(name)
	{
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		layers.emplace(layers.begin() + layerIndex, layer);
		layerIndex++;
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto it = std::ranges::find(layers, layer);
		if (it != layers.end())
		{
			layers.erase(it);
			layerIndex--;
		}
	}

	void LayerStack::pushOverlay(Overlay* overlay)
	{
		layers.emplace_back(overlay);
	}

	void LayerStack::popOverlay(Overlay* overlay)
	{
		auto it = std::ranges::find(layers, overlay);
		if (it != layers.end())
			layers.erase(it);
	}
}
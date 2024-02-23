#pragma once
#include "Core/Base.h"  
#include "Event/Event.h"
#include "Util/Util.h"

namespace Patchouli
{
	// Layers can be attached to or detached from the applicaiton,
	// responsible for updating, rendering, and handling events.
	class PATCHOULI_API Layer : public FastAllocated
	{
	public:
		Layer(const char* name = "New Layer");
		virtual ~Layer() = default;

		// Called when the layer is attached to the layer stack.
		virtual void onAttach() {}
		// Called when the layer is detached from the layer stack.
		virtual void onDetach() {}
		// Called every frame to update the layer.
		virtual void onUpdate() {}
		// Called to handle events for the layer.
		virtual void onEvent(Event& evt) {}

		const char* getName() const { return name; }
	private:
		const char* name;
	};

	// An overlay is essentially a specialized type of layer,
	// often used for UI and debugging features.
	using Overlay = Layer;

	// LayerStack is respoinsible for managing layers and overlays.
	class PATCHOULI_API LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack() = default;

		// Add a layer to the layer stack.
		void pushLayer(Layer* layer);
		// Remove a layer from the layer stack.
		void popLayer(Layer* layer);
		// Add a overlay to the layer stack.
		void pushOverlay(Overlay* overlay);
		// Remove a layer from the layer stack.
		void popOverlay(Overlay* overlay);
		// Get the beginning iterator for the layer stack.
		auto begin() const { return layers.begin(); }
		// Get the ending iterator for the layer stack.
		auto end() const { return layers.end(); }
		// Get the beginning const iterator for the layer stack.
		auto cbegin() const { return layers.cbegin(); }
		// Get the ending const iterator for the layer stack.
		auto cend() const { return layers.cend(); }

		std::size_t size() const { return layers.size(); }
	private:
		std::vector<Layer*> layers;
		uint32_t layerIndex = 0;
	};
}


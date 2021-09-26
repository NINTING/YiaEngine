#include "pch.h"
#include "Layer.h"

namespace YiaEngine
{
	void YiaEngine::LayerStack::PushLayer(Layer*layer)
	{
		insertIterator_ = stack_.emplace(insertIterator_,layer);
		insertIterator_++;
	}

	void YiaEngine::LayerStack::PushLayerOverlay(Layer* layer)
	{
		stack_.emplace_back(layer);
	}

	void YiaEngine::LayerStack::PopLayer(Layer*layer)
	{
		auto it = std::find(stack_.begin(), stack_.end(), layer);
		if (it != stack_.end())
		{
			stack_.erase(it);
			insertIterator_--;
		}
	}

	void YiaEngine::LayerStack::PopLayerOverlay(Layer* layer)
	{
		auto it = std::find(stack_.begin(), stack_.end(), layer);
		if (it != stack_.end())
		{
			stack_.erase(it);
		}
	}
	
}
#pragma once

#include"pch.h"
#include"Event/Event.h"
#include"Common/TimeStep.h"
namespace YiaEngine
{
	class Layer
	{
	public:
		Layer(const char* Name = "Layer") :debugName_(Name) {};
		virtual void OnEvent(Event& e) {};
		virtual void OnUpdate(Timestep timestep) {};
		virtual void OnAwake() {};
		virtual void OnDestroy() {};
	private:
		std::string debugName_;
	};


	class LayerStack
	{
	public:
		void PushLayer(Layer*);
		void PushLayerOverlay(Layer*);
		void PopLayer(Layer*);
		void PopLayerOverlay(Layer*);
		std::vector<Layer*>::iterator Begin() {return stack_.begin();}
		std::vector<Layer*>::iterator End() {return stack_.end(); }
	private:
		std::vector<Layer*>stack_;
		std::vector<Layer*>::iterator insertIterator_ = stack_.end();;
	};

}
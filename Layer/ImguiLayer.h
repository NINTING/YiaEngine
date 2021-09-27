#pragma once
#include "Layer/Layer.h"
#include"Core/DescriptorHeap.h"
namespace YiaEngine
{
	class ImguiLayer :public Layer
	{
	public:
		ImguiLayer();// : Layer("ImGuiLayer") {}
		virtual void OnAwake();
		virtual void OnEvent(Event& e);
		virtual void OnUpdate();
		virtual void OnDestroy();
	private:
		bool OnMouseMoveEvent(MouseMoveEvent& e);
		bool OnMouseDownEvent(MouseButtonDownEvent& e);
		bool OnMouseReleaseEvent(MouseButtonReleaseEvent& e);
		bool OnMouseWheelEvent(MouseWheelEvent& e);
	private:
		Graphic::DescriptorHeap gpuImGuiDescriptoHeap_;
		UINT64 time_;
	};

}
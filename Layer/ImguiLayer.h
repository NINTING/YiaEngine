#pragma once
#include "Layer/Layer.h"
#include"Core/YiaGraphic.h"

#include"ThirdParty/imgui/imgui.h"


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
	
		void Begin();
		void End();
		void Render();


		void Image(const Graphic::GpuTexture& resouece,const ImVec2 & size);
	private:
		bool OnMouseMoveEvent(MouseMoveEvent& e);
		bool OnMouseDownEvent(MouseButtonDownEvent& e);
		bool OnMouseReleaseEvent(MouseButtonReleaseEvent& e);
		bool OnMouseWheelEvent(MouseWheelEvent& e);
		bool OnWindowCloseEvent(WindowCloseEvent& e);
	private:
		
		void DockSpace();
		void SceneWindow();
	private:
		Graphic::DescriptorHeap gpuImGuiDescriptoHeap_;
		UINT64 time_;
		Graphic::GpuDescriptorAllocator imageHandleAllocator_;

		Graphic::RenderBuffer sceneColorBuffer;
		Graphic::DescriptorHandle sceneHandle;
	};

}
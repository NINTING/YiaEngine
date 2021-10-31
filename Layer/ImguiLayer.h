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
		virtual void OnUpdate(Timestep timestep);
		virtual void OnDestroy();
	
		void Begin();
		void End();
		void Render();

		Graphic::RenderBuffer& SceneColorBuffer() {return sceneColorBuffer;};
		Graphic::DepthBuffer& SceneDepthBuffer() { return sceneDepthBuffer; };
	private:
		bool OnMouseMoveEvent(MouseMoveEvent& e);
		bool OnMouseDownEvent(MouseButtonDownEvent& e);
		bool OnMouseReleaseEvent(MouseButtonReleaseEvent& e);
		bool OnMouseWheelEvent(MouseWheelEvent& e);
		bool OnWindowCloseEvent(WindowCloseEvent& e);
	private:
		void Image(const Graphic::GpuTexture& resouece, const ImVec2& size);
		void DockSpace();
		void SceneWindow();
	private:
		Graphic::DescriptorHeap gpuImGuiDescriptoHeap_;
		UINT64 time_;
		Graphic::GpuDescriptorAllocator imageHandleAllocator_;

		Graphic::RenderBuffer sceneColorBuffer;
		Graphic::DepthBuffer sceneDepthBuffer;
		Graphic::DescriptorHandle sceneHandle;
	};

}
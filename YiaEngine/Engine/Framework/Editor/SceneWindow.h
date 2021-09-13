#pragma once



#include "EditorWindow.h"

#include <d3d12.h>
#include<memory>

#include"Scene/Node/CameraNode.h"

#include"imgui.h"
#include"imgui_internal.h"
namespace YiaEngine
{
	class SceneWindow :public EditorWindow
	{
		D3D12_GPU_DESCRIPTOR_HANDLE TEMP_scene_handle_;
		std::unique_ptr<Scene::CameraNode> editor_camera_;
	public:
		
		SceneWindow(const char* name)
		{
			name_ = name;
		
		}

		void TEMP_SetShowRT(D3D12_GPU_DESCRIPTOR_HANDLE scene_gpu_handle)
		{
			TEMP_scene_handle_ = scene_gpu_handle;
		}
	private:
		virtual void Awake()
		{
			const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y + 20), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(600, 650), ImGuiCond_Always);
			window_flag_ |= ImGuiWindowFlags_NoMove;
		}
		virtual void BeforeBegin()
		{

		}
		
		virtual void OnGui()
		{
			ImGui::Image((ImTextureID)TEMP_scene_handle_.ptr, Vec2f(600, 600));
			ImGui::Image((ImTextureID)TEMP_scene_handle_.ptr, Vec2f(600, 600));
			//ImGui::GetCurrentContext()->CurrentWindow->DrawList->AddCallback();
		}
	};
}
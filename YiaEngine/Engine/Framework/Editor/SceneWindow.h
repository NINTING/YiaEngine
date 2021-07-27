#pragma once



#include "EditorWindow.h"
#include <d3d12.h>

namespace YiaEngine
{
	class SceneWindow :public EditorWindow
	{
		D3D12_GPU_DESCRIPTOR_HANDLE TEMP_scene_handle_;
	public:
		SceneWindow(std::string name)
		{
			name_ = name;
		}
		void TEMP_SetShowRT(D3D12_GPU_DESCRIPTOR_HANDLE scene_gpu_handle)
		{
			TEMP_scene_handle = scene_gpu_handle;
		}
	private:
		virtual void Awake()
		{
			ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y + 20), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(600, 650), ImGuiCond_Always);
			window_flags_ = ImGuiWindowFlags_NoMove;
		}
		virtual void BeforeBegin()
		{

		}
		
		virtual void OnGui()
		{
			ImGui::Image((ImTextureID)TEMP_scene_handle.ptr, Vec2f(600, 600));
		}
	};
}
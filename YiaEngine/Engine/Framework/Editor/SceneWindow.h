#pragma once



#include "EditorWindow.h"

#include <d3d12.h>
#include<memory>

#include"Scene/Node/CameraNode.h"

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
			//if (ImGui::IsWindowFocused())
			//{
			//	static Vec2f last_drag_delta = 0;
			//		if (YiaEngine::Input::IsKeyDown(YiaEngine::KeyCodeEnum::W))
			//		{
			//			editor_camera->MoveForward(Time::DetalTime*20);
			//		}
			//		if (YiaEngine::Input::IsKeyDown(YiaEngine::KeyCodeEnum::S))
			//		{
			//			editor_camera->MoveForward(-Time::DetalTime * 20);
			//		}
			//		if (YiaEngine::Input::IsKeyDown(YiaEngine::KeyCodeEnum::A))
			//		{
			//			editor_camera->Strafe(-Time::DetalTime * 20);
			//		}
			//		if (YiaEngine::Input::IsKeyDown(YiaEngine::KeyCodeEnum::D))
			//		{
			//			editor_camera->Strafe(Time::DetalTime * 20);
			//		}
			//		Vec2f mouse_delta = Input::MouseDragDelta();
			//		//mouse_delta = mouse_delta - last_drag_delta;
			//	//	last_drag_delta = 0;
			//		if (!mouse_delta.IsZero()) {
			//			/*mouse_delta.x /= g_width;
			//			mouse_delta.x *= 3.14;
			//			mouse_delta.y /= g_height;
			//			mouse_delta.y *= 3.14;*/
			//			//mouse_delta.x = std::fmaxf(mouse_delta.x, 10.f);
			//			mouse_delta.x =std::fmaxf(-30.f, std::fminf(mouse_delta.x, 30));
			//			mouse_delta.y = std::fmaxf(-30.f, std::fminf(mouse_delta.y, 30));
			//			//float delta_radians_x = Angle2Rad(mouse_delta.x * 0.1);
			//			float delta_radians_x = Angle2Rad(mouse_delta.x*0.01);
			//			float delta_radians_y = Angle2Rad(mouse_delta.y * 0.01);
			//			
			//			
			//			editor_camera->RotationEulerDebug(0, delta_radians_x, delta_radians_y);
			//		//	last_drag_delta = mouse_delta;
			//		}
			//}
		}
	};
}
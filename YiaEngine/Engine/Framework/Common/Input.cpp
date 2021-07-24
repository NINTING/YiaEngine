#pragma once

#include"External\ImGui\imgui.h"
#include"Input.h"
namespace YiaEngine
{
	bool Input::IsKeyDown(KeyCodeEnum key)
	{
		return ImGui::IsKeyDown((int)key);
		
	}
	Vec2f Input::MouseDragDelta()
	{
		return ImGui::GetMouseDragDelta();
	}
}
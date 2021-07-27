#pragma once

#include<string>

#include"ImGui.h"

namespace YiaEngine
{

	class EditorWindow
	{
		std::string name_;
		bool open_;
		ImGuiWindowFlags_ window_flag_ = ImGuiWindowFlags_None;
		virtual void OnGui() = 0;


		//before ImGui::Begin()
		virtual void BeforeBegin() {}

		//after ImGui::End();
		virtual void AfterEnd() {}

		//When the window show, initialize it once
		virtual void Awake() {}
	public:
		//pre frame call
		void Show()
		{
			static bool is_first_call = true;
			if (is_first_call) {
				Awake();
				is_first_call = false;
			}
			BeforeBegin();
			ImGui::Begin(name_, open_, window_flag_);
			OnGui();
			ImGui::End();
			AfterEnd();
		}
	};
}


#pragma once

namespace YiaEngine
{
	class Input
	{
	public:
		static void HandleMessage(void* msg);
		bool IsKeyPress(int keycode);
		virtual ~Input() { delete s_Instance; };
	protected:
		virtual void HandleMessageImpl(void* msg) = 0;
		virtual bool IsKeyPressedImpl(int keycode) = 0;
	private:
		static Input* s_Instance;
	};
}
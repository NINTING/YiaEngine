#pragma once

namespace YiaEngine
{
	class Input
	{
	public:
		bool IsKeyPress(int keycode);
	protected:
		virtual bool IsKeyPressedImpl(int keycode);
	private:
		static Input* s_Instance;
	};
}
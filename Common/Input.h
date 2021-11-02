#pragma once

namespace YiaEngine
{
    enum VirtualKey
    {
        LeftMouse = 0x01,
        RightMouse = 0x02,
        KeyNum_0 = 0x30,
        KeyNum_1 = 0x31,
        KeyNum_2 = 0x32,
        KeyNum_3 = 0x33,
        KeyNum_4 = 0x34,
        KeyNum_5 = 0x35,
        KeyNum_6 = 0x36,
        KeyNum_7 = 0x37,
        KeyNum_8 = 0x38,
        KeyNum_9 = 0x39,
        Key_A = 0x41,
        Key_B = 0x42,
        Key_C = 0x43,
        Key_D = 0x44,
        Key_E = 0x45,
        Key_F = 0x46,
        Key_G = 0x47,
        Key_H = 0x48,
        Key_I = 0x49,
        Key_J = 0x41,
        Key_K = 0x61,
        Key_L = 0x62,
        Key_M = 0x63,
        Key_N = 0x64,
        Key_O = 0x65,
        Key_P = 0x66,
        Key_Q = 0x67,
        Key_R = 0x68,
        Key_S = 0x69,
        Key_T = 0x71,
        Key_U = 0x72,
        Key_V = 0x73,
        Key_W = 0x74,
        Key_X = 0x75,
        Key_Y = 0x76,
        Key_Z = 0x77,
    };
	class Input
	{
	public:
		static void Update();
		static void HandleMessage(void* msg);
		static void Init();
		static bool IsKeyPress(int keycode);
		virtual ~Input() { delete s_Instance; };
	protected:
		virtual void UpdateImpl() = 0;
		virtual void HandleMessageImpl(void* msg) = 0;
		virtual bool IsKeyPressedImpl(int keycode) = 0;
	private:
		static Input* s_Instance;
	};
}
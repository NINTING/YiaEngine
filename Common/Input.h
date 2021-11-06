#pragma once
#include"Common/YiaMath.h"
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
        Key_J = 0x4A,
        Key_K = 0x4B,
        Key_L = 0x4C,
        Key_M = 0x4D,
        Key_N = 0x4E,
        Key_O = 0x4F,
        Key_P = 0x50,
        Key_Q = 0x51,
        Key_R = 0x52,
        Key_S = 0x53,
        Key_T = 0x54,
        Key_U = 0x55,
        Key_V = 0x56,
        Key_W = 0x57,
        Key_X = 0x58,
        Key_Y = 0x59,
        Key_Z = 0x5A,
    };
    class Input
    {
	public:
		static void Update();
		static void HandleMessage(void* msg);
		static void Init();
        static Math::Vec2f MouseMove();
		static bool IsKeyPress(int keycode);
		virtual ~Input() { delete s_Instance; };
	protected:
		virtual void UpdateImpl() = 0;
		virtual void HandleMessageImpl(void* msg) = 0;
		virtual bool IsKeyPressedImpl(int keycode) = 0;
    protected:
        Math::Vec2f mouseMove_;
        Math::Vec2f mousePos_;
    private:
		static Input* s_Instance;

       
	};
}
#pragma once
#include "Common/Input.h"
namespace YiaEngine
{

   

    class WindowInput :public Input
    {
    public:
        virtual void UpdateImpl();
    protected:
        virtual bool IsKeyPressedImpl(int keycode);
        virtual void HandleMessageImpl(void* msg);

    protected:
        char textBuffer_[32];
        UINT textCount_;
        bool keyButtonState[255];
    };
}

#pragma once
#include "Common/Input.h"
namespace YiaEngine
{
    class WindowInput :public Input
    {
    public:
        
    protected:
        virtual bool IsKeyPressedImpl(int keycode);
        virtual void HandleMessageImpl(void* msg);
    };
}

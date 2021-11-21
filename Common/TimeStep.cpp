#include "pch.h"
#include "TimeStep.h"



namespace YiaEngine
{
    Timestep* Timestep::s_TimeStep;

    void YiaEngine::Timestep::InitGlobalFrameTime()
    {
        s_TimeStep = new Timestep(0);
    }

    void YiaEngine::Timestep::Update(long milSecond)
    {
        s_TimeStep->deltaMilSecond_ = milSecond;
        s_TimeStep->deltaTime_ = (float)(milSecond / 1000.f);
    }

    float YiaEngine::Timestep::GetFrameSeconds()
    {
        return  s_TimeStep->deltaTime_;
    }

    float YiaEngine::Timestep::GetFrameMilliSeconds()
    {
        return s_TimeStep->deltaMilSecond_;
    }

}

#pragma once

namespace YiaEngine
{

	class Timestep
	{
	public:
		static void InitGlobalFrameTime();
		static void Update(long milSecond);
		static float GetFrameSeconds();
		static float GetFrameMilliSeconds();
	public:
		Timestep(long milSecond):deltaMilSecond_(milSecond), deltaTime_((float)milSecond/1000.f){}
		
		float GetSeconds()const { return deltaTime_; }
		float GetMillisecond()const { return deltaTime_ * 1000.f; }
	private:
		long deltaMilSecond_;
		float deltaTime_;
		static Timestep* s_TimeStep;
	};
}
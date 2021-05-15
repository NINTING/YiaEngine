#pragma once

#include<string>

namespace YiaEngine::Graghic
{
	class FramePass {
	public:
		std::string name
			virtual  void Setup() = 0;
		virtual  void Execute() = 0;
	};
}
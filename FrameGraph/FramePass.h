#pragma once

#include"pch.h"
#include "Core/YiaGraphic.h"
#include "FrameResource.h"
namespace YiaEngine
{
	namespace FrameGraph
	{
		
		/// <summary>
		/// 保存执行时数据，以及进行准备和执行函数
		/// 是Graph中的执行主体
		/// </summary>
		class FramePass
		{
		protected:
			virtual void Setup() = 0;
			virtual void Execute() = 0;
			void AddInput();
		
		};

		class PassNode
		{

		};

	}
}


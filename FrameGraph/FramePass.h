#pragma once

#include"pch.h"
#include "Core/YiaGraphic.h"
#include "FrameResource.h"

namespace YiaEngine
{
	namespace YiaFrameGraph
	{
		class FrameGraph;
		/// <summary>
		/// 保存执行时数据，以及进行准备和执行函数
		/// 是Graph中的执行主体
		/// </summary>
		class FramePass
		{
			FramePass(PassNode& passnode) :node_(passnode) {}
		protected:
			virtual void Setup(FrameGraph& fg) = 0;
			virtual void Execute() = 0;
			void AddInputResource(const VirtualResourceHandle& handle);
			void AddOutput(const VirtualResourceHandle& handle);
		private:
			PassNode& node_;
		};

		class PassNode
		{
		public:
			void AddInputResource(const VirtualResourceHandle& handl);
		private:
			std::vector<VirtualResourceHandle> read;
			std::vector<VirtualResourceHandle> write;
			int refCount;
		};

	}
}


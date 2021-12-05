//#pragma once
//
//#include"pch.h"
//#include "Core/YiaGraphic.h"
//#include "FrameResource.h"
//
//namespace YiaEngine
//{
//	namespace YiaFrameGraph
//	{
//		class FrameGraph;
//		/// <summary>
//		/// 保存执行时数据，以及进行准备和执行函数
//		/// 是Graph中的执行主体
//		/// </summary>
//		class FramePass
//		{
//		public:
//			FramePass(PassNode& passnode) :node_(passnode) {}
//			virtual void Setup(FrameGraph& fg) = 0;
//			virtual void Execute() = 0;
//		protected:
//		
//			void AddInputResource(const VirtualResourceHandle& handle);
//			VirtualResourceHandle AddOutput(FrameGraph& fg, const VirtualResourceHandle& handle);
//			void AddOutput(const VirtualResourceHandle& handle);
//			void SetInputPass(PassNode);
//		private:
//			PassNode& node_;
//		};
//
//		class PassNode
//		{
//
//		public:
//			PassNode(FramePass& pass) :framePass(pass) {}
//			void SetupPass(FrameGraph& fg) { framePass.Setup(fg); };
//			void ExecutePass() { framePass.Execute();  };
//			void AddInputResource(const VirtualResourceHandle& handl);
//			void AddOutputResource(const VirtualResourceHandle& handl);
//			int ReadCount() { return reads.size(); };
//			int WriteCount() { return writes.size(); };
//			std::vector<VirtualResourceHandle>
//				GetReadList() { return reads; };
//			std::vector<VirtualResourceHandle>
//				GetWriteList() { return writes; };
//			void ComputeRef();
//			int GetRef() { return refCount; }
//			int GetSideEffect() { return sideEffect; }
//			VirtualResourceHandle GetOutputHandle(const char*name);
//
//		private:
//			std::vector<VirtualResourceHandle> reads;
//			std::vector<VirtualResourceHandle> writes;
//			int refCount;
//			bool sideEffect;
//			FramePass& framePass;
//			std::vector<PassNode>nextList;
//		};
//
//	}
//}
//

#include "pch.h"
//#include "FrameGraph.h"
//#include"Core/YiaGraphic.h"
//namespace YiaEngine
//{
//	namespace YiaFrameGraph
//	{
//
//		VirtualResourceHandle  FrameGraph::CreateTemplateRenderBuffer(const char* name, Graphic::TextureDescribe desc)
//		{
//			int resIndex = resouceList_.size();
//			VirtualResourceHandle handle;
//			handle.Id = resouceNodeList_.size();
//			
//			namedPool.insert(name);
//			handle.NameHash = namedPool.hash_function()(name);
//			
//			ResourceNode resNode(resIndex);
//			resouceNodeList_.emplace_back(resNode);
//			resouceList_.emplace_back(std::make_unique<VirtualResource>(name,desc));
//			resourceIdMap_[name] = handle.Id;
//			
//			return handle;
//		}
//
//		//VirtualResourceHandle  FrameGraph::RegisterResource(const char* name, VirtualResourceHandle handle)
//		//{
//		//	ResourceNode resNode(resIndex);
//		//	resouceNodeList_.emplace_back(resNode);
//		//	resourceIdMap_[handle.NameHash] = handle.Id;
//		//}
//
//		VirtualResourceHandle FrameGraph::RegisterResource(const char* name, std::shared_ptr<Graphic::GpuTexture> texture)
//		{
//			
//			resouceList_.emplace_back(std::make_unique<VirtualResource>(name, texture));
//			resourceIdMap_[name] = resouceList_.size()-1;
//			
//			
//
//			return AddResourceNode(name);
//		}
//
//		VirtualResourceHandle FrameGraph::AddResourceNode(const VirtualResourceHandle& handle)
//		{
//			VirtualResourceHandle ret;
//			ResourceNode node(resourceIdMap_[handle.NameHash]);
//
//			ret.NameHash = handle.NameHash;
//			ret.Id = resouceNodeList_.size();
//			resouceNodeList_.emplace_back(node);
//			return ret;
//		}
//
//		VirtualResourceHandle FrameGraph::AddResourceNode(const char* name)
//		{
//			StrHash h = namedPool.hash_function()(name);
//			VirtualResourceHandle handle;
//
//			if (resourceIdMap_.find(name)==resourceIdMap_.end())
//			{
//				YIA_ERROR("Not Found Resource {0}",name);
//				YIA_ERROR("Register Virtual Resource Before AddResourceNode!!!");
//				return handle;
//			}
//			
//			handle.Id = resouceNodeList_.size();
//			namedPool.insert(name);
//
//			ResourceNode resNode(resourceIdMap_[name]);
//			resouceNodeList_.emplace_back(resNode);
//
//			return handle;
//		}
//
//		VirtualResourceHandle FrameGraph::GetResourceHandle(const char* name)
//		{
//			StrHash NameHash = namedPool.hash_function()(name);
//			return VirtualResourceHandle(NameHash,-1);
//		}
//
//
//		VirtualResource FrameGraph::GetResource(const char* name)
//		{
//
//		}
//
//		const PassNode& FrameGraph::GetPass(const char* name)
//		{
//			return passNodeList_[passIdMap_[name]];
//		}
//
//		ResourceNode& FrameGraph::GetResourceNode(const VirtualResourceHandle& handle)
//		{
//			return resouceNodeList_[handle.Id];
//		}
//		void FrameGraph::AddPass(const char* name, FramePass& framePass)
//		{
//		
//			passNodeList_.emplace_back(PassNode(framePass));
//			passIdMap_[name] = passNodeList_.size() - 1;
//		}
//
//
//
//		void FrameGraph::Setup()
//		{
//			for (int i = 0;i<passNodeList_.size();i++)
//			{
//				passNodeList_[i].SetupPass(*this);
//			}
//		}
//
//		/*
//		* �޳�����
//		* 1. ����������޸�����
//		* 
//		*/
//		void FrameGraph::Compile()
//		{
//			/*
//			* first step
//			* compute Ref
//			*/
//			for (PassNode& passNode : passNodeList_)
//			{
//				auto readList = passNode.GetReadList();
//				for (auto handle : readList)
//				{
//					auto& node = GetResourceNode(handle);
//					node.ReadCount++;
//					node.RefCount++;
//				}
//
//				auto readList = passNode.GetReadList();
//				auto writeList = passNode.GetReadList();
//				for (auto handle : readList)
//				{
//					auto& node = GetResourceNode(handle);
//					node.ReadCount++;
//					node.RefCount++;
//				}
//
//				for (auto handle : writeList)
//				{
//					auto& node = GetResourceNode(handle);
//					node.RefCount++;
//				}
//				
//			}
//
//			/*�޳�
//			* 
//			* 1.û��pass�����ResourceNode����鿴д������pass��
//			* 2.���д��passû�и�������ֻд���resource�����ù�0��������pass�Ķ���resource��
//			* 3.����pass��resource���޳�
//			* 4.�ظ�����1
//			* 
//			* 
//			*/
//			std::vector<ResourceNode*> invalidStack;
//			for (int i = 0; i < resouceNodeList_.size(); i++)
//			{
//				if (resouceNodeList_[i].ReadCount == 0)
//				{
//					invalidStack.push_back(&resouceNodeList_[i]);
//				}
//			}
//			while (invalidStack.empty())
//			{
//				ResourceNode* node = invalidStack.back();
//				invalidStack.pop_back();
//				
//				if (node->WritePass)
//				{
//					if (!node->WritePass->GetSideEffect() && node->WritePass->WriteCount() == 1)
//					{
//						auto readList = node->WritePass->GetReadList();
//						for (int i = 0; i < readList.size(); i++)
//						{
//							ResourceNode& node = GetResourceNode(readList[i]);
//							if (--node.RefCount == 0)
//							{
//								invalidStack.push_back(&node);
//							}
//						}
//					}
//				}
//			}
//
//			/*����pass node������*/
//			for (PassNode& passNode : passNodeList_)
//			{
//				passNode.ComputeRef();
//			}
//
//			/*������Դ��ʼ���Լ�����ʱ��*/
//			//�ⲿע����Դ����֡��Դ�����ٻ򴴽�
//			for (PassNode& passNode : passNodeList_)
//			{
//				if (passNode.GetRef()==0)
//				{
//					continue;
//				}
//				auto writes = passNode.GetWriteList();
//				auto reads = passNode.GetReadList();
//				
//				for (int i = 0; i < reads.size(); i++)
//				{
//
//				}
//
//				for (int i = 0;i<writes.size();i++)
//				{                          
//
//				}
//				
//			}
//		}
//
//
//		void FrameGraph::Execute()
//		{
//
//		}
//
//	}
//}
//
